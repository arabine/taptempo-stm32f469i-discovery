/*
 * Copyright (c) 2012, 2013, Joel Bodenmann aka Tectu <joel@unormal.org>
 * Copyright (c) 2012, 2013, Andrew Hannam aka inmarket
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the name of the <organization> nor the
 *      names of its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "gfx.h"

static GListener gl;
static GHandle   ghButton1;

gdispImage            recLogo;
gdispImage            linuxFrLogo;

gdispImage            digit0;
gdispImage            digit1;
gdispImage            digit2;
gdispImage            digit3;
gdispImage            digit4;
gdispImage            digit5;
gdispImage            digit6;
gdispImage            digit7;
gdispImage            digit8;
gdispImage            digit9;

static GHandle ghLabel1;

GTimer GT1;

gdispImage *GetDigit(int value)
{
	gdispImage *img = &digit0;
	switch(value)
	{
	case 1:
		img = &digit1;
		break;
	case 2:
		img = &digit2;
		break;
	case 3:
		img = &digit3;
		break;
	case 4:
		img = &digit4;
		break;
	case 5:
		img = &digit5;
		break;
	case 6:
		img = &digit6;
		break;
	case 7:
		img = &digit7;
		break;
	case 8:
		img = &digit8;
		break;
	case 9:
		img = &digit9;
		break;
	default:
		break;
	}
	return img;
}

void DisplayBigNumber(int value)
{
	// saturate
	if (value > 999)
	{
		value = 999;
	}

	int cent = value / 100;
	int tenth = (value / 10) - (cent*10);
	int unit = value - (value/10)*10;

	gdispImageDraw(GetDigit(cent), 0, 240, 100, 200, 0, 0);
	gdispImageDraw(GetDigit(tenth), 100, 240, 100, 200, 0, 0);
	gdispImageDraw(GetDigit(unit), 200, 240, 100, 200, 0, 0);
}

#define RESET_TIME 3

int tapCounter = 0;
int elapsedTime = 0;
int secondCounter = 0;
int resetTime = 0;
int memoriseCounter = 0;

void callback1(void* arg)
{
    (void)arg;

    elapsedTime++;
	// Refresh every second
	if (++secondCounter >= 10)
	{
		secondCounter = 0;
		if (tapCounter != memoriseCounter)
		{
			memoriseCounter = tapCounter; resetTime = 0;
		}
		else
		{
			// detect reset (silence)
			if (++resetTime >= RESET_TIME)
			{
				resetTime = 0;
				tapCounter = 0;
				elapsedTime = 0;
			}
		}
		int bmp = 0;

		if (tapCounter)
		{
			int meanTime = (elapsedTime*100) / tapCounter;

			if (meanTime)
			{
				bmp = 60000 / meanTime;
			}
		}
		DisplayBigNumber(bmp);
	}
}

static void createWidgets(void) {
	GWidgetInit	wi;

	// Apply some default values for GWIN
	gwinWidgetClearInit(&wi);
	wi.g.show = TRUE;


//	// Apply the label parameters
//	wi.g.y = 10;
//	wi.g.x = 10;
//	wi.g.width = 0; // 0 = auto
//	wi.g.height = 0; // 0 = auto
//	wi.text = "1";
//
//	// Create the actual label
//	ghLabel1 = gwinLabelCreate(NULL, &wi);


	// Apply the button parameters
	wi.g.width = 200;
	wi.g.height = 200;
	wi.g.y = 200;
	wi.g.x = 500;
	wi.text = "TapTempo!";

	// Create the actual button
	ghButton1 = gwinButtonCreate(0, &wi);
}

void showSplash(void)
{
   // gdispImageOpenFile(&recLogo, "logo.bmp");
   // gdispImageDraw(&recLogo, (gdispGetWidth()/2)-225, 0, 453, 93, 0, 0);


	gdispImageOpenFile(&linuxFrLogo, "linuxfr.bmp");
	gdispImageDraw(&linuxFrLogo, 0, 0, 200, 185, 0, 0);

    gdispImageClose(&recLogo);
}


int main(void) {
	GEvent* pe;
	static const orientation_t	orients[] = { GDISP_ROTATE_0, GDISP_ROTATE_90, GDISP_ROTATE_180, GDISP_ROTATE_270 };
	unsigned which;


	// Initialize the display
	gfxInit();

	// We are currently at GDISP_ROTATE_0
	which = 0;
	gdispSetOrientation(orients[which]);

	// Set the widget defaults
	gwinSetDefaultFont(gdispOpenFont("UI2"));
	gwinSetDefaultStyle(&WhiteWidgetStyle, TRUE);
	gdispClear(Black);

	// create the widget
	createWidgets();

	gtimerInit(&GT1);
	/* continious mode - callback1() called without any argument every 1000ms */
	gtimerStart(&GT1, callback1, NULL, TRUE, 100);

	showSplash();

	gdispImageOpenFile(&digit0, "0.bmp");
	gdispImageOpenFile(&digit1, "1.bmp");
	gdispImageOpenFile(&digit2, "2.bmp");
	gdispImageOpenFile(&digit3, "3.bmp");
	gdispImageOpenFile(&digit4, "4.bmp");
	gdispImageOpenFile(&digit5, "5.bmp");
	gdispImageOpenFile(&digit6, "6.bmp");
	gdispImageOpenFile(&digit7, "7.bmp");
	gdispImageOpenFile(&digit8, "8.bmp");
	gdispImageOpenFile(&digit9, "9.bmp");

	DisplayBigNumber(0);

	// We want to listen for widget events
	geventListenerInit(&gl);
	gwinAttachListener(&gl);


//	font_t		font1;
//	font1 = gdispOpenFont("*");
//
//	gdispGFillStringBox(m->display,
//									0, CALIBRATION_TITLE_Y, w, CALIBRATION_TITLE_HEIGHT,
//									CALIBRATION_TITLE, font1,  CALIBRATION_TITLE_COLOR, CALIBRATION_TITLE_BACKGROUND,
//									justifyCenter);

	while(1) {
		// Get an Event
		pe = geventEventWait(&gl, TIME_INFINITE);

		switch(pe->type) {
			case GEVENT_MOUSE:
//				gwinSetText(ghLabel1, "Touch detected!", TRUE);
			break;

			case GEVENT_GWIN_BUTTON:
				if (((GEventGWinButton*)pe)->gwin == ghButton1) {
					// Our button has been pressed
//					if (++which >= sizeof(orients)/sizeof(orients[0]))
//						which = 0;

					// Setting the orientation during run-time is a bit naughty particularly with
					// GWIN windows. In this case however we know that the button is in the top-left
					// corner which should translate safely into any orientation.
//					gdispSetOrientation(orients[which]);
//					gdispClear(White);
//					gwinRedrawDisplay(GDISP, FALSE);

					if (!tapCounter)
					{
						elapsedTime = 0;
					}
					tapCounter++;
//					gwinSetText(ghLabel1, "Tap!!!", TRUE);
				}
				break;

			default:
				break;
		}
	}

	return 0;
}

