/******************************************************************************
 * Project:  PROJ.4
 * Purpose:  Internal plumbing for the PROJ.4 library.
 *
 * Author:   Thomas Knudsen, <thokn@sdfe.dk>
 *
 ******************************************************************************
 * Copyright (c) 2016, 2017, Thomas Knudsen / SDFE
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO COORD SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *****************************************************************************/

#ifdef _MSC_VER
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#endif
#include <math.h>   /* For M_PI */

#include <proj.h>

#ifdef PROJECTS_H
#error proj_internal.h must be included before projects.h
#endif
#ifdef PROJ_API_H
#error proj_internal.h must be included before proj_api.h
#endif

#ifndef PROJ_INTERNAL_H
#define PROJ_INTERNAL_H
#ifdef __cplusplus
extern "C" {
#endif

#ifndef PJ_TODEG
#define PJ_TODEG(rad)  ((rad)*180.0/M_PI)
#endif
#ifndef PJ_TORAD
#define PJ_TORAD(deg)  ((deg)*M_PI/180.0)
#endif

/* This enum is also conditionally defined in projects.h - but we need it here */
/* for the pj_left/right prototypes, and enums cannot be forward declared      */
enum pj_io_units {
    PJ_IO_UNITS_CLASSIC = 0,   /* Scaled meters (right) */
    PJ_IO_UNITS_METERS  = 1,   /* Meters  */
    PJ_IO_UNITS_RADIANS = 2    /* Radians */
};
enum pj_io_units pj_left (PJ *P);
enum pj_io_units pj_right (PJ *P);

PJ_COORD   proj_trans   (PJ *P, PJ_DIRECTION direction, PJ_COORD obs);

PJ_COORD proj_coord_error (void);

void proj_context_errno_set (PJ_CONTEXT *ctx, int err);
void proj_context_set (PJ *P, PJ_CONTEXT *ctx);
void proj_context_inherit (PJ *parent, PJ *child);

PJ_COORD pj_fwd4d (PJ_COORD coo, PJ *P);
PJ_COORD pj_inv4d (PJ_COORD coo, PJ *P);

/* Grid functionality */
int             proj_vgrid_init(PJ *P, const char *grids);
int             proj_hgrid_init(PJ *P, const char *grids);
double          proj_vgrid_value(PJ *P, LP lp);
LP              proj_hgrid_value(PJ *P, LP lp);
LP              proj_hgrid_apply(PJ *P, LP lp, PJ_DIRECTION direction);

/* High level functionality for handling thread contexts */
enum proj_log_level {
    PJ_LOG_NONE  = 0,
    PJ_LOG_ERROR = 1,
    PJ_LOG_DEBUG = 2,
    PJ_LOG_TRACE = 3,
    PJ_LOG_TELL  = 4,
    PJ_LOG_DEBUG_MAJOR = 2, /* for proj_api.h compatibility */
    PJ_LOG_DEBUG_MINOR = 3  /* for proj_api.h compatibility */
};

/* Set logging level 0-3. Higher number means more debug info. 0 turns it off */
enum proj_log_level proj_log_level (PJ_CONTEXT *ctx, enum proj_log_level log_level);
typedef void (*PJ_LOG_FUNCTION)(void *, int, const char *);

void proj_log_error (PJ *P, const char *fmt, ...);
void proj_log_debug (PJ *P, const char *fmt, ...);
void proj_log_trace (PJ *P, const char *fmt, ...);
/*void proj_log_func (PJ *P, void *app_data, void (*log)(void *, int, const char *));*/
void proj_log_func (PJ_CONTEXT *ctx, void *app_data, PJ_LOG_FUNCTION log);

void pj_inherit_ellipsoid_defs(const PJ *src, PJ *dst);
int pj_calc_ellps_params(PJ *P, double a, double es);

/* Lowest level: Minimum support for fileapi */
void proj_fileapi_set (PJ *P, void *fileapi);

const char **proj_get_searchpath(void);
int    proj_get_path_count(void);

size_t pj_strlcpy(char *dst, const char *src, size_t siz);

#ifdef __cplusplus
}
#endif

#endif /* ndef PROJ_INTERNAL_H */
