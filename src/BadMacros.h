// These macros must be defined for the code to compile
// They are awful and should never have been created
// and should be cast into the fires of Mount Doom

#ifndef BAD_MACROS_H
#define BAD_MACROS_H

// No idea what this could possibly be for; some kind of lexical pasting?
// GWM: Yes it is for lexical pasting. So that ICE_HEADER(ICE_EDITION.h)
// becomes "Ice/Ice.h".
#define ICE_EDITION Ice

#define QUOTE_IT(x) #x

// Makes ICE_HEADER(header) into "Ice/header"
#define ICEUTIL_HEADER(header) QUOTE_IT(IceUtil / header)
#define ICE_HEADER(header) QUOTE_IT(Ice / header)

#endif