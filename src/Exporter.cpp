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
    // Get the frame size, and pointer to start
    size_t nbytes = sizeFrame(frame);
    const mfm::Byte *pData = frame.data();

    // Convert frame to raw buffer
    std::vector<uint8_t> buffer;
    buffer.insert(buffer.end(), &((uint8_t *)pData)[0], &((uint8_t *)pData)[nbytes - 1]);
    DataExporter::ServerMessage message(buffer);

    // Write the frame to disk
    FrameStorage::processFrame(frame);

    // If the server died, for now ignore the frame
    if (!m_server.IsActive())
    {
        return;
    }
    // Send the frame over the server
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