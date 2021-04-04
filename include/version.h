#pragma once

/// @group
/// @todo Extract version number from build system.
#define MAJOR 0
#define MINOR 1
#define PATCH 0
/// @endgroup

#define STR_(x) #x
#define STR(x) STR_(x)
#define VERSION_STRING "v" STR(MAJOR) "." STR(MINOR) "." STR(PATCH)
