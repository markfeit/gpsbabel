/*
	st2gpx.h

	Extract data from MS Streets & Trips .est, Autoroute .axe 
	and Mapoint .ptm files in GPX format.

    Copyright (C) 2003 James Sherring, james_sherring@yahoo.com

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111 USA


	This app depends on istorage & istorage-make from Pabs (pabs3@zip.to)
	and James Clark's Expat xml parser from http://www.libexpat.org/.

*/
#ifdef	__cplusplus
extern "C" {
#endif

//#define EXPLORE

#ifdef _DEBUG
#define MEMCHK
#define DEBUG_STDOUT

#define _CRTDBG_MAP_ALLOC
#define  SET_CRT_DEBUG_FIELD(a)   _CrtSetDbgFlag((a) | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#define  CLEAR_CRT_DEBUG_FIELD(a) _CrtSetDbgFlag(~(a) & _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#else
#define  SET_CRT_DEBUG_FIELD(a)   ((void) 0)
#define  CLEAR_CRT_DEBUG_FIELD(a) ((void) 0)
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef unsigned short WCHAR;

struct st2gpx_options
{
	// 0 - only errors
	// 1 also the structured data output, e.g. line point info
	// 2 also record types & parameters
	// 3 also detailed summary info
	// 4 also detailed analysis of headers & record params
	// 5 also dump buffers
	// 6 lots of debug info
	int verbose_flag;
	int explore_flag;
	// force line-type annotations to be exported as GPX routes instead of tracks
	int use_gpx_route;
	char* source_file_name;
	// wait for enter key at certain points in the program
	int debug_wait_flag;
	int st_version_num;
	WCHAR* MapName;
	// 0 for EUR, i.e. Autoroute
	// 1 for USA, i,e, Streets & Trips
	unsigned char isUSA;
};


// FIXME is this the correct way to forward define these?
#ifndef	__cplusplus

extern struct point;
extern struct pushpin;
extern struct annotations;
extern struct pushpin_safelist;
extern struct annot_rec ;
extern struct gpx_data;
extern struct journey;
extern struct contents;

#endif // __cplusplus

//st2gpx.c
extern struct st2gpx_options opts;
void * xmalloc(size_t size);
void * xrealloc(void* ptr, size_t size);
char * str2ascii(char* str);
char * strappend(char* str1, char* str2);
char * buf2str(char* buf, int strlen);
char * buf2wstr(char* buf, int strlen);
int readbytes(FILE* file, char* buf, int bytes2read);
//nannol.c
struct annotations * merge_gpx_annot(struct annotations * annots, struct gpx_data* all_gpx);
void write_annotations(struct annotations * annots, char* annot_out_file_name);
//writepcx.c
void pcx5_export(char* pcx5_out_file_name, struct pushpin_safelist * ppplist, struct journey * jour, struct annotations * pannots);
//debug.c
void debug_pause();
void printbuf(char* buf, int len);
void debug_show_sizes();
//st2gpx.c
struct gpx_data * read_mpstext(char* mpstxt_file_name);

#ifdef	__cplusplus
}
#endif

