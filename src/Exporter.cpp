/** @file:  Exporter.cpp
 *  @brief: Implements a TCP server fro GRAW Frames
 */

#include "Exporter.h"
#include "server/ServerMessage.h"
#include <mfm/Frame.h>
#include <mfm/Field.h>
#include <mfm/Header.h>
#include <mfm/Common.h>

Exporter::Exporter(uint16_t serverPort) : FrameStorage(), m_server(serverPort)
{
}

Exporter::~Exporter()
{
}

void Exporter::processFrame(mfm::Frame &frame)
{
    // Get file
    uint32_t coboIdx, asadIdx, eventIdx;
    SourceUID sourceUID;

    // Sanity checks on the frame status...
    try
    {
        coboIdx = frame.headerField("coboIdx").value<uint8_t>();
        asadIdx = frame.headerField("asadIdx").value<uint8_t>();
        sourceUID = std::make_pair<>(coboIdx, asadIdx);
        eventIdx = frame.headerField("eventIdx").value<uint32_t>();
        LOG_DEBUG() << "Storing AsAd no. " << asadIdx << " frame for event " << std::dec << eventIdx << " (" << frame.header().frameSize_B() << " B)";
    }
    catch (const mfm::Exception &)
    {
        // Check if frame is of MuTanT type 0
        if (frame.header().frameType() == 0x8)
        {
            sourceUID = std::make_pair<>(100, 0);
            eventIdx = frame.headerField(14u, 4u).value<uint32_t>();
            LOG_DEBUG() << "Storing MuTanT frame for event " << std::dec << eventIdx << " (" << frame.header().frameSize_B() << " B)";
        }
        else
        {
            LOG_WARN() << "Dumping frame of type " << std::hex << std::showbase << frame.header().frameType() << std::dec;
            return;
        }
    }

    // Store frame in file
    std::ofstream &file = getFile(sourceUID, eventIdx);
    frame.write(file);

    // Deal with write errors (no space left on device)
    if (not file.good())
    {
        LOG_ERROR() << "Error writing frame to file!";
        closeFile(sourceUID);
    }
    else
    {
        // Close file after recording its size
        const size_t fileSize_B = file.tellp();
        closeFile(sourceUID);

        // Create new file if maximum file size has been reached
        if (fileSize_B > maxFileSize_MiB * 0x100000)
        {
            createNewFile("", sourceUID, false);
        }
    }

    // If the server died, for now ignore the frame

    if (!m_server.IsActive())
    {
        return;
    }

    // Get the frame size, and pointer to start
    size_t nbytes = sizeFrame(frame);
    const mfm::Byte *pData = frame.data();

    // Convert frame to raw buffer
    std::vector<uint8_t> buffer;
    buffer.insert(buffer.end(), &((uint8_t *)pData)[0], &((uint8_t *)pData)[nbytes - 1]);
    DataExporter::ServerMessage message(buffer);

    m_server.MessageClients(message);
}

/*-----------------------------------------------------------------------------
 *  Private utilities.
 */
/**
 * sizeFrame
 *    If I understand the Frame class correctly,   What we need to do
 *    is get a const reference to the header (supported by Frame).  Ask the
 *    header to tell us its size and the size of the frame body that follows.
 *
 *  @param frame - const reference to the frame we're dissecting.
 *  @return size_t - size of the frame + header all in one.  I hope this
 *                   includes the meta type field. else we'll miss a byte.
 */
size_t
Exporter::sizeFrame(const mfm::Frame &frame)
{
    mfm::Header const &h = frame.header();
    size_t result = h.headerSize_B() + h.dataSize_B();

    return result;
}