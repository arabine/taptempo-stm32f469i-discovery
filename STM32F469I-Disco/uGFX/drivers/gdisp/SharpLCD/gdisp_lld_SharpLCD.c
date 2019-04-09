/*
 * This file is subject to the terms of the GFX License. If a copy of
 * the license was not distributed with this file, you can obtain one at:
 *
 *              http://ugfx.org/license.html
 */

#include "gfx.h"

#if GFX_USE_GDISP

#define GDISP_DRIVER_VMT			GDISPVMT_SHARPLCD
#include "gdisp_lld_config.h"
#include "../../../src/gdisp/gdisp_driver.h"

#include "board_SharpLCD.h"
#include "SQ013B7DH03.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#ifndef GDISP_SCREEN_HEIGHT
	#define GDISP_SCREEN_HEIGHT		128
#endif
#ifndef GDISP_SCREEN_WIDTH
	#define GDISP_SCREEN_WIDTH		128
#endif

/* Erase everything in the frame buffer and set all the pixels to white. */
void ClearFrameBuffer(GDisplay *g){
  for(int i=0;i<((GDISP_SCREEN_WIDTH * GDISP_SCREEN_HEIGHT) + GDISP_SCREEN_HEIGHT)/8;i++){
      ((uint8_t *)g->priv)[i] = 0xFF; // 0xFF = white
  }
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

LLDSPEC bool_t gdisp_lld_init(GDisplay *g) {
	/* The frame buffer will be stored here. One bit per pixel 128x128 pixels. */
    /* Indexing is pretty easy (Note that the pixel index starts with 0!):
     * |  pix0  |  pix1  | ... | pix126 | pix127 | -> First line is indexed 1 - 128 (or 0+pixel)
     * | pix128 | pix129 | ... | pix254 | pix255 | -> Second line is indexed 128+pixel
     * ...
     * |pix16256|pix16257| ... |pix16387|pix16383| -> Last line 16129+pixel
     * So indexing is done: SCREEN_WIDTH*line + row (line index is 0 to (SCREEN_HEIGHT-1) and row 0 to (SCREEN_WIDTH-1))
     * At the end of the frame buffer there is a buffer that indicates that a line has changed in the buffer.
     */
	g->priv = gfxAlloc((GDISP_SCREEN_WIDTH * GDISP_SCREEN_HEIGHT) + GDISP_SCREEN_HEIGHT);
	if (!g->priv) {
	  return FALSE;
	}

	/* The frame buffer is filled with black to make the startup LOGO look good. */
  for(int i=0;i<((GDISP_SCREEN_WIDTH * GDISP_SCREEN_HEIGHT) + GDISP_SCREEN_HEIGHT)/8;i++){
      ((uint8_t *)g->priv)[i] = 0x00; // 0x00 = black
  }

	// Initialize the board interface
	init_board(g);

  // Finish Init
  post_init_board(g);

	/* Initialize the GDISP structure */
	g->g.Width = GDISP_SCREEN_WIDTH;
	g->g.Height = GDISP_SCREEN_HEIGHT;
	g->g.Orientation = GDISP_ROTATE_0;
	return TRUE;
}

#if GDISP_HARDWARE_DRAWPIXEL
    LLDSPEC void gdisp_lld_draw_pixel(GDisplay *g) {
      coord_t       x, y;
      /* Take screen rotation into account. */
      switch(g->g.Orientation) {
      default:
      case GDISP_ROTATE_0:
        x = g->p.x;
        y = g->p.y;
        break;
      case GDISP_ROTATE_90:
        x = g->p.y;
        y = GDISP_SCREEN_HEIGHT-1 - g->p.x;
        break;
      case GDISP_ROTATE_180:
        x = GDISP_SCREEN_WIDTH-1 - g->p.x;
        y = GDISP_SCREEN_HEIGHT-1 - g->p.y;
        break;
      case GDISP_ROTATE_270:
        x = GDISP_SCREEN_HEIGHT-1 - g->p.y;
        y = g->p.x;
        break;
      }
      /* There is only black and no black (white). */
      if (gdispColor2Native(g->p.color) != Black) // Indexing in the array is done as described in the init routine
        ((uint8_t *)g->priv)[((GDISP_SCREEN_WIDTH/8)*y) + (x/8)] |= (1 << (x%8));
      else
        ((uint8_t *)g->priv)[((GDISP_SCREEN_WIDTH/8)*y) + (x/8)] &= ~(1 << (x%8));

      ((uint8_t *)g->priv)[((GDISP_SCREEN_HEIGHT * GDISP_SCREEN_WIDTH)/8) + (y/8)] |= (1 << (y%8)); // Invalidate the line
    }
#endif

#if GDISP_HARDWARE_FLUSH
    LLDSPEC void gdisp_lld_flush(GDisplay *g) {
      /* Acquire the bus and send the command indicating lines are written to the display. */
      acquire_bus(g);
      write_data(g, WRITE_LINE_CMD);
      /* Start writing frame buffer to ram. */
      for(int i=0;i<GDISP_SCREEN_HEIGHT;i++){
          if(((uint8_t *)g->priv)[((GDISP_SCREEN_HEIGHT * GDISP_SCREEN_WIDTH)/8) + (i/8)] & (1 << (i%8))){
            write_data(g, (i+1)); // Line number
            for(int j=0;j<(GDISP_SCREEN_WIDTH/8);j++)
              write_data(g, ((uint8_t *)g->priv)[((GDISP_SCREEN_HEIGHT/8)*i) + j]); // Pixel data
            write_data(g, 0x00);  // 8 dummy bits that the display needs
            ((uint8_t *)g->priv)[((GDISP_SCREEN_HEIGHT * GDISP_SCREEN_WIDTH)/8) + (i/8)] &= ~(1 << (i%8)); // Reset invalidation
          }
      }
      write_data(g, 0x00);  // 16 dummy bits that the display needs
      write_data(g, 0x00);
      release_bus(g);
    }
#endif

#if GDISP_NEED_CONTROL && GDISP_HARDWARE_CONTROL
	LLDSPEC void gdisp_lld_control(GDisplay *g) {
		switch(g->p.x) {
		case GDISP_CONTROL_POWER:
			if (g->g.Powermode == (powermode_t)g->p.ptr)
				return;
			switch((powermode_t)g->p.ptr) {
			case powerOff:
				break;
			case powerOn:
				break;
			case powerSleep:
				break;
			default:
				return;
			}
			g->g.Powermode = (powermode_t)g->p.ptr;
			return;

		case GDISP_CONTROL_ORIENTATION:
			if (g->g.Orientation == (orientation_t)g->p.ptr)
				return;
			switch((orientation_t)g->p.ptr) {
			case GDISP_ROTATE_0:
				g->g.Height = GDISP_SCREEN_HEIGHT;
				g->g.Width = GDISP_SCREEN_WIDTH;
				break;
			case GDISP_ROTATE_90:
				g->g.Height = GDISP_SCREEN_WIDTH;
				g->g.Width = GDISP_SCREEN_HEIGHT;
				break;
			case GDISP_ROTATE_180:
				g->g.Height = GDISP_SCREEN_HEIGHT;
				g->g.Width = GDISP_SCREEN_WIDTH;
				break;
			case GDISP_ROTATE_270:
				g->g.Height = GDISP_SCREEN_WIDTH;
				g->g.Width = GDISP_SCREEN_HEIGHT;
				break;
			default:
				return;
			}
			g->g.Orientation = (orientation_t)g->p.ptr;
			return;
        default:
            return;
		}
	}
#endif

/* Hardware clears are supported by the display. */
#if GDISP_HARDWARE_CLEARS
    void gdisp_lld_clear(GDisplay *g) {
        acquire_bus(g);
        write_data(g, CLEAR_SCREEN_CMD);
        write_data(g, 0x00);
        release_bus(g);
        ClearFrameBuffer(g); // We need to clear the frame buffer also
    }
#endif

#endif /* GFX_USE_GDISP */
