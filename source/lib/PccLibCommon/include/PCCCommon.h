/* The copyright in this software is being made available under the BSD
 * License, included below. This software may be subject to other third party
 * and contributor rights, including patent rights, and no such rights are
 * granted under this license.
 *
 * Copyright (c) 2010-2017, ISO/IEC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *  * Neither the name of the ISO/IEC nor the names of its contributors may
 *    be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef PCCTMC2Common_h
#define PCCTMC2Common_h

#define NOMINMAX
#include <limits>
#include <assert.h>
#include <chrono>
#include <cmath>
#include <vector>
#include <list>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <cstring>
#include <unordered_map>
#include <sys/stat.h>
#include <memory>
#include <queue>
#include <algorithm>
#include <map>
#include <array>
#include "PCCConfig.h"
#include "PCCBitstreamCommon.h"
#if defined( WIN32 )
#include <windows.h>
#endif
#if defined( __APPLE__ ) && defined( __MACH__ )
#include <unistd.h>
#include <sys/resource.h>
#include <mach/mach.h>
#endif
#ifdef ENABLE_PAPI_PROFILING
#include <papi.h>
#include "PCCChrono.h"
#endif
#undef OPTIONAL

#define MATCHTOANCHOR 1  // jkei: set 1 to match with the results of anchor for mapcount=1 case.
#define AUXVIDEO_TILE \
  1  // tile7: if(auxiliaryVideoTileRowHeight_.size()!=other.auxiliaryVideoTileRowHeight_.size()) return false;

#define MULTISTREAM_BUGFIX 1
#define LOSSYOCCMAP_BUGFIX 1
#define REFERENCELIST_BUGFIX 1
#define USEAUXVIDEOFLAG 1
#define MULTISTREAM_AUXVIDEO 1

#define TILEINFO_DERIVATION 1  // d127
#define NONUNIFORM_PARTSIZE 0

#define LOSSLESS_3DROI 1

namespace pcc {

// ******************************************************************* //
// Version information
// ******************************************************************* //
#define TM2_VERSION TMC2_VERSION_MAJOR "." TMC2_VERSION_MINOR

// ******************************************************************* //
// Coding tool configuration
// ******************************************************************* //
static const uint8_t PCC_SAVE_POINT_TYPE = 1;  // Save point information in reconstructed ply.

// ******************************************************************* //
// Trace modes to validate new syntax
// ******************************************************************* //
#define CODEC_TRACE

// ******************************************************************* //
// Common constants
// ******************************************************************* //
#define POSTSMOOTHING_RGB2YUV 0

enum PCCEndianness { PCC_BIG_ENDIAN = 0, PCC_LITTLE_ENDIAN = 1 };
enum PCCColorTransform { COLOR_TRANSFORM_NONE = 0, COLOR_TRANSFORM_RGB_TO_YCBCR = 1 };
enum PCCPointType { POINT_UNSET = 0, POINT_D0, POINT_D1, POINT_DF, POINT_SMOOTH, POINT_EOM, POINT_RAW };
enum { COLOURFORMAT420 = 0, COLOURFORMAT444 = 1 };
enum PCCCOLORFORMAT { UNKNOWN = 0, RGB444, YUV444, YUV420 };

enum PCCCodecId {
#ifdef USE_JMAPP_VIDEO_CODEC
  JMAPP = 0,
#endif
#ifdef USE_HMAPP_VIDEO_CODEC
  HMAPP = 1,
#endif
#ifdef USE_HMLIB_VIDEO_CODEC
  HMLIB = 2,
#endif
#ifdef USE_FFMPEG_VIDEO_CODEC
  FFMPEG = 3,
#endif
  UNKNOWN_CODEC = 4
};

const int16_t infiniteDepth          = ( std::numeric_limits<int16_t>::max )();
const int64_t infinitenumber         = ( std::numeric_limits<int64_t>::max )();
const int32_t InvalidPatchIndex      = -1;
const size_t  IntermediateLayerIndex = 100;
const size_t  eomLayerIndex          = 10;
const size_t  NeighborThreshold      = 4;
const size_t  NumPatchOrientations   = 8;
const size_t  gbitCountSize[]        = {
    0, 0, 1, 0, 2, 0, 0, 0,  // 0-7
    3, 0, 0, 0, 0, 0, 0, 0,  // 8-
    4, 0, 0, 0, 0, 0, 0, 0,  // 16-
    0, 0, 0, 0, 0, 0, 0, 0,  // 24-
    5, 0, 0, 0, 0, 0, 0, 0,  // 32-
    0, 0, 0, 0, 0, 0, 0, 0,  // 40
    0, 0, 0, 0, 0, 0, 0, 0,  // 48
    0, 0, 0, 0, 0, 0, 0, 0,  // 56
    6                        // 64
};

const std::vector<int> orientation_vertical = {
    PATCH_ORIENTATION_DEFAULT, PATCH_ORIENTATION_SWAP,    PATCH_ORIENTATION_ROT180,
    PATCH_ORIENTATION_MIRROR,  PATCH_ORIENTATION_MROT180, PATCH_ORIENTATION_ROT270,
    PATCH_ORIENTATION_MROT90,  PATCH_ORIENTATION_ROT90};  // favoring vertical orientation
const std::vector<int> orientation_horizontal = {PATCH_ORIENTATION_SWAP,   PATCH_ORIENTATION_DEFAULT,
                                                 PATCH_ORIENTATION_ROT270, PATCH_ORIENTATION_MROT90,
                                                 PATCH_ORIENTATION_ROT90,  PATCH_ORIENTATION_ROT180,
                                                 PATCH_ORIENTATION_MIRROR, PATCH_ORIENTATION_MROT180};  // favoring
                                                                                                        // horizontal
                                                                                                        // orientations
                                                                                                        // (that should
                                                                                                        // be rotated)

static inline PCCEndianness PCCSystemEndianness() {
  uint32_t num = 1;
  return ( *( reinterpret_cast<char*>( &num ) ) == 1 ) ? PCC_LITTLE_ENDIAN : PCC_BIG_ENDIAN;
}

static inline void PCCDivideRange( const size_t         start,
                                   const size_t         end,
                                   const size_t         chunckCount,
                                   std::vector<size_t>& subRanges ) {
  const size_t elementCount = end - start;
  if ( elementCount <= chunckCount ) {
    subRanges.resize( elementCount + 1 );
    for ( size_t i = start; i <= end; ++i ) { subRanges[i - start] = i; }
  } else {
    subRanges.resize( chunckCount + 1 );
    const double step = static_cast<double>( elementCount ) / ( chunckCount + 1 );
    double       pos  = static_cast<double>( start );
    for ( size_t i = 0; i < chunckCount; ++i ) {
      subRanges[i] = static_cast<size_t>( pos );
      pos += step;
    }
    subRanges[chunckCount] = end;
  }
}

template <typename... Args>
std::string stringFormat( const char* pFormat, Args... eArgs ) {
  size_t      iSize = snprintf( NULL, 0, pFormat, eArgs... );
  std::string eBuffer;
  eBuffer.reserve( iSize + 1 );
  eBuffer.resize( iSize );
  snprintf( &eBuffer[0], iSize + 1, pFormat, eArgs... );
  return eBuffer;
}

template <typename T>
void printVector( std::vector<T>    data,
                  const size_t      width,
                  const size_t      height,
                  const std::string string,
                  const bool        hexa = false ) {
  if ( data.size() == 0 ) { data.resize( width * height, 0 ); }
  printf( "%s: %zu %zu \n", string.c_str(), width, height );
  for ( size_t v0 = 0; v0 < height; ++v0 ) {
    for ( size_t u0 = 0; u0 < width; ++u0 ) {
      if ( hexa ) {
        printf( "%2x", (int)( data[v0 * width + u0] ) );
      } else {
        printf( "%3d", (int)( data[v0 * width + u0] ) );
      }
    }
    printf( "\n" );
    fflush( stdout );
  }
}

static inline std::string getParameter( const std::string& config, const std::string& param ) {
  std::size_t pos = 0;
  if ( ( pos = config.find( param ) ) == std::string::npos ) {
    printf( "Can't find parameter: \"%s\" in: %s \n", param.c_str(), config.c_str() );
    exit( -1 );
  }
  pos += param.length();
  return config.substr( pos, config.find( " ", pos ) - pos );
}

using Range = std::pair<int, int>;
struct Tile {
  int minU;
  int maxU;
  int minV;
  int maxV;
  Tile() : minU( -1 ), maxU( -1 ), minV( -1 ), maxV( -1 ){};
};

#ifdef ENABLE_PAPI_PROFILING
#define ERROR_RETURN( retval )                                              \
  fprintf( stderr, "Error %d %s:line %d: \n", retval, __FILE__, __LINE__ ); \
  exit( retval );

static void initPapiProfiler() {
  int  retval;
  char errstring[PAPI_MAX_STR_LEN];
  if ( ( retval = PAPI_library_init( PAPI_VER_CURRENT ) ) != PAPI_VER_CURRENT ) { ERROR_RETURN( retval ); }
  return;
}

static void createPapiEvent( int& EventSet ) {
  EventSet = PAPI_NULL;
  int retval, number = 0;
  if ( ( retval = PAPI_create_eventset( &EventSet ) ) != PAPI_OK ) { ERROR_RETURN( retval ); }
  if ( ( retval = PAPI_add_event( EventSet, PAPI_TOT_INS ) ) != PAPI_OK ) { ERROR_RETURN( retval ); }
  if ( ( retval = PAPI_add_event( EventSet, PAPI_TOT_CYC ) ) != PAPI_OK ) { ERROR_RETURN( retval ); }
  if ( ( retval = PAPI_add_event( EventSet, PAPI_L2_TCA ) ) != PAPI_OK ) { ERROR_RETURN( retval ); }
  if ( ( retval = PAPI_add_event( EventSet, PAPI_L3_TCA ) ) != PAPI_OK ) { ERROR_RETURN( retval ); }
  if ( ( retval = PAPI_add_event( EventSet, PAPI_L1_DCM ) ) != PAPI_OK ) { ERROR_RETURN( retval ); }
  if ( ( retval = PAPI_add_event( EventSet, PAPI_L2_DCM ) ) != PAPI_OK ) { ERROR_RETURN( retval ); }
  if ( ( retval = PAPI_list_events( EventSet, NULL, &number ) ) != PAPI_OK ) { ERROR_RETURN( retval ); }
  return;
}

#define PAPI_PROFILING_INITIALIZE                          \
  int       EventSet = PAPI_NULL;                          \
  int       retval;                                        \
  long long values[16];                                    \
  createPapiEvent( EventSet );                             \
  pcc::chrono::Stopwatch<std::chrono::steady_clock> clock; \
  clock.reset();                                           \
  clock.start();                                           \
  if ( ( retval = PAPI_start( EventSet ) ) != PAPI_OK ) { ERROR_RETURN( retval ); }

#define PAPI_PROFILING_RESULTS                                                                    \
  clock.stop();                                                                                   \
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( clock.count() ).count(); \
  if ( ( retval = PAPI_read( EventSet, values ) ) != PAPI_OK ) { ERROR_RETURN( retval ); }        \
  printf( "PAPI: number of instructions           : %lld \n", values[0] );                        \
  printf( "PAPI: number of cycles                 : %lld \n", values[1] );                        \
  printf( "PAPI: number of L2 cache memory access : %lld \n", values[2] );                        \
  printf( "PAPI: number of L3 cache memory access : %lld \n", values[3] );                        \
  printf( "PAPI: number of L1 cache misses        : %lld \n", values[4] );                        \
  printf( "PAPI: number of L2 cache misses        : %lld \n", values[5] );                        \
  printf( "PAPI: Processing time (wall)           : %lld \n", duration );
#endif
}  // namespace pcc

#endif /* PCCTMC2Common_h */
