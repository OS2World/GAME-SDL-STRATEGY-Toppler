
#include "txtsys.h"
#include "screen.h"
#include "keyb.h"
#include "menu.h"
#include <stdlib.h>
#include <string.h>

textsystem::textsystem(char *title, menuopt_callback_proc pr)
{
    if (title) {
	this->title = (char *)malloc(strlen(title)+1);
	strcpy(this->title, title);
    } else this->title = NULL;

    this->numlines = 0;
    this->max_length = 0;
    this->lines = NULL;
    this->mproc = pr;
    this->timeproc = NULL;
    this->curr_mtime = 0;
    this->mtime = -1;
    this->xoffs = this->yoffs = this->disp_xoffs = this->disp_yoffs = 0;
    this->key = SDLK_UNKNOWN;
    this->ystart = (title) ? FONTHEI + 15 : 0;
    this->shownlines = ((SCREENHEI - this->ystart) / FONTHEI) + 1;
}

textsystem::~textsystem()
{
    int i;

    if (this->lines && this->numlines) {
	for (i = 0; i < this->numlines; i++)
	  if (this->lines[i]) 
	    free(this->lines[i]);
	free(this->lines);
    }
    if (this->title) free(this->title);
}

void textsystem::addline(char *line)
{
   char **tmp;
   int olen;
   tmp = (char **)malloc(sizeof(char *)*(this->numlines+1));
    
   if (!tmp) return;

   if (this->lines) {
      memcpy(tmp, this->lines, sizeof(char *)*this->numlines);
      free(this->lines);
   }

   if (line && (strlen(line)>0)) {
       tmp[this->numlines] = (char *)malloc(strlen(line)+1);
       strcpy(tmp[this->numlines], line);
   } else tmp[this->numlines] = NULL;

   this->lines = tmp;

   this->numlines++;

   if (line) {
       olen = scr_formattextlength(0,0,line);
       if (olen < 0) olen = 0;
   }
   else olen = 0;

   if (this->max_length < olen) this->max_length = olen;
}

void textsystem::set_timer(long t, menuopt_callback_proc pr)
{
    this->timeproc = pr;
    this->mtime = t;
}


void textsystem::run()
{
    bool ende = false;
    
    do {
	(void)key_readkey();

	this->draw();
	
	this->key = key_sdlkey();
      
	if (key_keypressed(quit_action)) break;

	switch (key_sdlkey2conv(this->key, false)) {
	    case up_key:
	        if (this->yoffs >= FONTHEI) this->yoffs -= FONTHEI;
	        else this->yoffs = 0;
	        break;
	    case down_key:
	        if (this->yoffs + (this->shownlines*FONTHEI) < (this->numlines*FONTHEI)) this->yoffs += FONTHEI;
	        else this->yoffs = (this->numlines - this->shownlines+1)*FONTHEI;
	        break;
	    case break_key: ende = true; break;
	    case left_key:
	        if (this->xoffs >= FONTWID) this->xoffs -= FONTWID; 
	        else this->xoffs = 0; 
	        break;
	    case right_key: 
	        if (this->xoffs <= this->max_length-SCREENWID-FONTWID) this->xoffs += FONTWID;
		else this->xoffs = this->max_length-SCREENWID;
	        break;
	    default:
	        switch (this->key) {
		    case SDLK_PAGEUP:
		        if (this->yoffs >= this->shownlines*FONTHEI) this->yoffs -= this->shownlines*FONTHEI;
		        else this->yoffs = 0;
		        break;
		    case SDLK_SPACE:
		    case SDLK_PAGEDOWN:
		        if ((this->yoffs/FONTHEI) + (this->shownlines*2) <= this->numlines)
		           this->yoffs += this->shownlines*FONTHEI;
		        else this->yoffs = (this->numlines - this->shownlines+1)*FONTHEI;
		        break;
		    case SDLK_HOME:   this->yoffs = 0; break;
		    case SDLK_END:    this->yoffs = (this->numlines - this->shownlines+1)*FONTHEI; break;
		    case SDLK_RETURN:
		    case SDLK_ESCAPE: ende = true; break;
		    default: break;
		}
	}

    } while (!ende);
}

void
textsystem::draw()
{
    char pointup[2], pointdown[2], pointleft[2], pointright[2];

    pointup[0] = fontptrup;
    pointup[1] = 0;
    pointdown[0] = fontptrdown;
    pointdown[1] = 0;
    pointleft[0] = fontptrleft;
    pointleft[1] = 0;
    pointright[0] = fontptrright;
    pointright[1] = 0;

    if (this->mproc) {
	(*this->mproc) (NULL);
//	menu_background_proc = NULL;
    }

    if (this->title)
      scr_writetext_center(5, this->title);

    if (this->disp_yoffs < this->yoffs) {
	this->disp_yoffs += ((this->yoffs - this->disp_yoffs+3) / 4)+1;
	if (this->disp_yoffs > this->yoffs) this->disp_yoffs = this->yoffs;
    } else if (this->disp_yoffs > this->yoffs) {
	this->disp_yoffs -= ((this->disp_yoffs - this->yoffs+3) / 4)+1;
	if (this->disp_yoffs < this->yoffs) this->disp_yoffs = this->yoffs;
    }

    if (this->disp_xoffs < this->xoffs) {
	this->disp_xoffs += ((this->xoffs - this->disp_xoffs) / 4)+1;
	if (this->disp_xoffs > this->xoffs) this->disp_xoffs = this->xoffs;
    } else if (this->disp_xoffs > this->xoffs) {
	this->disp_xoffs -= ((this->disp_xoffs - this->xoffs) / 4)+1;
	if (this->disp_xoffs < this->xoffs) this->disp_xoffs = this->xoffs;
    }

    scr_setclipping(0, this->ystart, SCREENWID, SCREENHEI);
    for (int k = 0; k <= this->shownlines; k++)
      if (k+(this->disp_yoffs / FONTHEI) < this->numlines) {
	  if (this->lines[k+(this->disp_yoffs / FONTHEI)])
	    scr_writeformattext(-this->disp_xoffs,
			  k*FONTHEI + this->ystart - (this->disp_yoffs % FONTHEI),
			  this->lines[k+(this->disp_yoffs / FONTHEI)]);
      }

    scr_setclipping();

    if (this->disp_yoffs > 0)
      scr_writetext(SCREENWID-FONTWID, 34, pointup);
    if ((this->disp_yoffs / FONTHEI) + this->shownlines < this->numlines)
      scr_writetext(SCREENWID-FONTWID, SCREENHEI-FONTHEI, pointdown);
    
    if (this->disp_xoffs > 0)
      scr_writetext(FONTWID, 5, pointleft);
    if (this->disp_xoffs < this->max_length - SCREENWID)
      scr_writetext(SCREENWID-FONTWID, 5, pointright);

    scr_swap();
    dcl_wait();
}
