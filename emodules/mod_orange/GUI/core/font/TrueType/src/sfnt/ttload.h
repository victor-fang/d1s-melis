/*
* Copyright (c) 2019-2025 Allwinner Technology Co., Ltd. ALL rights reserved.
*
* Allwinner is a trademark of Allwinner Technology Co.,Ltd., registered in
* the the People's Republic of China and other countries.
* All Allwinner Technology Co.,Ltd. trademarks are used with permission.
*
* DISCLAIMER
* THIRD PARTY LICENCES MAY BE REQUIRED TO IMPLEMENT THE SOLUTION/PRODUCT.
* IF YOU NEED TO INTEGRATE THIRD PARTY’S TECHNOLOGY (SONY, DTS, DOLBY, AVS OR MPEGLA, ETC.)
* IN ALLWINNERS’SDK OR PRODUCTS, YOU SHALL BE SOLELY RESPONSIBLE TO OBTAIN
* ALL APPROPRIATELY REQUIRED THIRD PARTY LICENCES.
* ALLWINNER SHALL HAVE NO WARRANTY, INDEMNITY OR OTHER OBLIGATIONS WITH RESPECT TO MATTERS
* COVERED UNDER ANY REQUIRED THIRD PARTY LICENSE.
* YOU ARE SOLELY RESPONSIBLE FOR YOUR USAGE OF THIRD PARTY’S TECHNOLOGY.
*
*
* THIS SOFTWARE IS PROVIDED BY ALLWINNER"AS IS" AND TO THE MAXIMUM EXTENT
* PERMITTED BY LAW, ALLWINNER EXPRESSLY DISCLAIMS ALL WARRANTIES OF ANY KIND,
* WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING WITHOUT LIMITATION REGARDING
* THE TITLE, NON-INFRINGEMENT, ACCURACY, CONDITION, COMPLETENESS, PERFORMANCE
* OR MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
* IN NO EVENT SHALL ALLWINNER BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
* NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS, OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
* OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/*                                                                         */
/*  ttload.h                                                               */
/*                                                                         */
/*    Load the basic TrueType tables, i.e., tables that can be either in   */
/*    TTF or OTF fonts (specification).                                    */
/*                                                                         */
/*  Copyright 1996-2001, 2002, 2005, 2006 by                               */
/*  David Turner, Robert Wilhelm, and Werner Lemberg.                      */
/*                                                                         */
/*  This file is part of the FreeType project, and may only be used,       */
/*  modified, and distributed under the terms of the FreeType project      */
/*  license, LICENSE.TXT.  By continuing to use, modify, or distribute     */
/*  this file you indicate that you have read the license and              */
/*  understand and accept it fully.                                        */
/*                                                                         */
/***************************************************************************/
#ifndef __TTLOAD_H__
#define __TTLOAD_H__


#include "ft2build.h"
#include FT_INTERNAL_STREAM_H
#include FT_INTERNAL_TRUETYPE_TYPES_H


FT_BEGIN_HEADER


FT_LOCAL(TT_Table)
tt_face_lookup_table(TT_Face   face,
                     FT_ULong  tag);

FT_LOCAL(FT_Error)
tt_face_goto_table(TT_Face    face,
                   FT_ULong   tag,
                   FT_Stream  stream,
                   FT_ULong  *length);


FT_LOCAL(FT_Error)
tt_face_load_font_dir(TT_Face    face,
                      FT_Stream  stream);


FT_LOCAL(FT_Error)
tt_face_load_any(TT_Face    face,
                 FT_ULong   tag,
                 FT_Long    offset,
                 FT_Byte   *buffer,
                 FT_ULong  *length);


FT_LOCAL(FT_Error)
tt_face_load_head(TT_Face    face,
                  FT_Stream  stream);


FT_LOCAL(FT_Error)
tt_face_load_cmap(TT_Face    face,
                  FT_Stream  stream);


FT_LOCAL(FT_Error)
tt_face_load_maxp(TT_Face    face,
                  FT_Stream  stream);


FT_LOCAL(FT_Error)
tt_face_load_name(TT_Face    face,
                  FT_Stream  stream);


FT_LOCAL(FT_Error)
tt_face_load_os2(TT_Face    face,
                 FT_Stream  stream);


FT_LOCAL(FT_Error)
tt_face_load_post(TT_Face    face,
                  FT_Stream  stream);


FT_LOCAL(FT_Error)
tt_face_load_pclt(TT_Face    face,
                  FT_Stream  stream);

FT_LOCAL(void)
tt_face_free_name(TT_Face  face);


FT_LOCAL(FT_Error)
tt_face_load_gasp(TT_Face    face,
                  FT_Stream  stream);

#ifdef TT_CONFIG_OPTION_EMBEDDED_BITMAPS

FT_LOCAL(FT_Error)
tt_face_load_bhed(TT_Face    face,
                  FT_Stream  stream);

#endif /* TT_CONFIG_OPTION_EMBEDDED_BITMAPS */


FT_END_HEADER

#endif /* __TTLOAD_H__ */


/* END */
