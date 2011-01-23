/*
 * This file is part of the xMangaPSP application.
 *
 * Copyright (C) Luqman Aden <www.luqmanrocks.co.cc>.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * Full License: LICENSE.txt
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * Header for LeX Engine.
 * 
 * @package xMangaPSP
 */

#ifndef _LeXEngine_H
#define _LeXEngine_H

// For compatibility
#ifndef O_BINARY
#define O_BINARY 0
#endif

// BEGIN Includes
#include "xMPStandard.h"

#include <psputility.h>

#include "pgeMp3.h"

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_rotozoom.h>
#include <SDL/SDL_thread.h>
#include <SDL/SDL_ttf.h>
#include "SDL_rwops_zzip.h"

// Net Stuff
#include <curl/curl.h>
#include <curl/easy.h>

#include "FastEvents.h"

#include "Timer.h"

#include <vector>
// END Includes

/**
 * Useful struct used when converting SDL_Surfaces.
 */
typedef struct {

	GLuint texture;
	GLfloat minX;
	GLfloat maxX;
	GLfloat minY;
	GLfloat maxY;
	int w, pw;
	int h, ph;

} xMTexture;

class State;

/**
 * Init flags.
 */
enum LeXInitFlags {

	/**
	 * Init everything. {Basically means all flags}
	 */
	LeXInitEVERYTHING	=	1,
	
	/**
	 * Init OpenGL through SDL.
	 */
	LeXInitOpenGL		=	2,
	
	/**
	 * Init OpenGL Double Buffering.
	 */
	LeXInitOpenGLDB		=	4,
	
	/**
	 * Init the NET stuff.
	 */
	LeXInitNET			=	8,
	
	/**
	 * Init Audio.
	 */
	LeXInitAUDIO		=	16,
	
	/**
	 * Init SDL Joystick stuff.
	 */
	LeXInitJOYSTICK		=	32,
	
	/**
	 * Start SDL with no cursor.
	 */
	LeXInitNOCURSOR		=	64,
		
	/**
	 * Init libCurl. {Also use LeXInitNET with this flag.}
	 */
	LeXInitCURL			=	128,
	
	/**
	 * Init SDL TrueType font library.
	 */
	LeXInitTTF			=	256,
	
	/**
	 * Don't init anything other than Video and Timers.
	 * Prevents other flags from working.
	 */
	LeXInitNOEXTRA		=	512

};

class LeXEngine {

	private:
		
		/**
		 * The surface representing the screen.
		 * 
		 * @access private
		 */
		SDL_Surface *screen;
		
		/**
		 * Flags to determine which systems to startup and shutdown.
		 * 
		 * @access private.
		 */
		int initFlags;
		
		/**
		 * SDL Joysticks.
		 * 
		 * @access private
		 */
		std::vector<SDL_Joystick*> joysticks;
		
		/**
		 * The screen width.
		 * 
		 * @access private
		 */
		int screenWidth;
		
		/**
		 * The screen height.
		 * 
		 * @access private
		 */
		int screenHeight;
		
		/**
		 * The Bits-Per-Pixel.
		 * 
		 * @access private
		 */
		int bitsPerPixel;
		
		/**
		 * SDL Video Mode flags.
		 * 
		 * @access private
		 */
		int videoModeFlags;
				
		/**
		 * Is the game running?
		 * 
		 * @access private
		 */
		bool isRunning;
		
		/**
		 * Stack of states.
		 * 
		 * @access private
		 */
		std::vector<State*> states;

		/**
		 * Init systems.
		 * 
		 * @access private
		 * 
		 * @param int screenWidth The screen width.
		 * @param int screenHeight The screen height.
		 * @param int bitsPerPixel Bits-per-pixel.
		 * @param LeXInitFlags Startup flags.
		 * @param Uint32 SDL Video mode flags.
		 */
		void init(int screenWidth, int screenHeight, int bitsPerPixel, int initFlags, Uint32 videoModeFlags);
		
		/**
		 * Clean up music stuff.
		 * 
		 * @access private
		 */
		void cleanUpMusic();
				
	public:
	
		/**
		 * Constructor.
		 * Init systems.
		 *
		 * @access public
		 * 
		 * @param int screenWidth The screen width.
		 * @param int screenHeight The screen height.
		 * @param int bitsPerPixel Bits-per-pixel.
		 */
		LeXEngine(int screenWidth, int screenHeight, int bitsPerPixel);
		
		/**
		 * Constructor.
		 * Init systems.
		 *
		 * @access public
		 * 
		 * @param int screenWidth The screen width.
		 * @param int screenHeight The screen height.
		 * @param int bitsPerPixel Bits-per-pixel.
		 * @param LeXInitFlags Startup flags.
		 */
		LeXEngine(int screenWidth, int screenHeight, int bitsPerPixel, int initFlags);
				
		/**
		 * Constructor.
		 * Init systems.
		 *
		 * @access public
		 * 
		 * @param int screenWidth The screen width.
		 * @param int screenHeight The screen height.
		 * @param int bitsPerPixel Bits-per-pixel.
		 * @param LeXInitFlags Startup flags.
		 * @param Uint32 SDL Video mode flags.
		 */
		LeXEngine(int screenWidth, int screenHeight, int bitsPerPixel, int initFlags, Uint32 videoModeFlags);
		
		/**
		 * Destructor.
		 * Shutdown systems.
		 */
		~LeXEngine();
						
		/**
		 * Setup the screen.
		 * 
		 * @access public
		 */
		void setupScreen();
		
		/**
		 * Sets up OpenGL environment.
		 * 
		 * @access public
		 */
		void setupOpenGL();
		
		/**
		 * Is game running?
		 * 
		 * @access public
		 * 
		 * @return bool Running?
		 */
		bool isGameRunning();
		
		/**
		 * Change isRunning to true. Use before loop.
		 * 
		 * @access public
		 */
		void startGame();
		
		/**
		 * Change isRunning to false. Use in loop.
		 * 
		 * @access public
		 */
		void quitGame();
		
		/**
		 * Return the current state.
		 * 
		 * @access public
		 * 
		 * @return State* The current state.
		 */
		State* getCurrentState();
		
		/**
		 * Takes a pointer to a State as a parameter and then pushes that state onto the vector of pointers to States, before that it uses the clean function to remove the old state from the stack.
		 * 
		 * @access public
		 * 
		 * @param State* state The new state.
		 */
		void changeState(State* state);
		
		/**
		 * Pauses the previous state before pushing a new state onto the stack, this state can then be removed and the previous state resumed.
		 * Extrememly useful for pausing.
		 * 
		 * @access public
		 * 
		 * @param State* The new state.
		 */
		void pushState(State* state);
		
		/**
		 * Remove and resume previous state.
		 * 
		 * @access public
		 */
		void popState();
		
		/**
		 * Process user input.
		 * 
		 * @access public
		 */
		void handleInput();
		
		/**
		 * Perform logic and update state of program.
		 * 
		 * @access public
		 */
		void handleLogic();
		
		/**
		 * Draw onto screen.
		 * 
		 * @access public
		 */
		void render();
		
		/**
		 * Update screen.
		 * 
		 * @access public
		 */
		void updateScreen();
		
		/**
		 * Get screen.
		 * 
		 * @access public
		 * 
		 * @return SDL_Surface* The screen.
		 */
		SDL_Surface* getScreen();

		/**
		 * Set screen.
		 * 
		 * @access public
		 * 
		 * @param SDL_Surface* screen The new screen.
		 */
		void setScreen(SDL_Surface* screen);
		
		/**
		 * Get Screen height.
		 * 
		 * @access public
		 * 
		 * @return int Screen height.
		 */
		int getScreenHeight();
		
		/**
		 * Get Screen width.
		 * 
		 * @access public
		 * 
		 * @return int Screen width.
		 */
		int getScreenWidth();
						
		/**
		 * Apply surface.
		 * 
		 * @access public
		 * 
		 * @param int x X position.
		 * @param int y Y position.
		 * @param SDL_Surface source The source surface.
		 * @param SDL_Surface destination The destination surface.
		 * @param SDL_Rect[Optional] clip Portion or NULL.
		 */
		void sdlApplySurface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL);
		
		/**
		 * Apply surface directly to screen.
		 * 
		 * @access public
		 * 
		 * @param int x X position.
		 * @param int y Y position.
		 * @param SDL_Surface source The source surface.
		 * @param SDL_Rect[Optional] clip Portion or NULL.
		 */
		void sdlApplySurfaceToScreen(int x, int y, SDL_Surface* source, SDL_Rect* clip = NULL);
		
		/**
		 * Scales an SDL Surface to the requested width and height with SDL_gfx.
		 * 
		 * @access public
		 * 
		 * @param SDL_Surface src Original surface.
		 * @param double newWidth The new width.
		 * @param double newHeight The new height.
		 * 
		 * @return SDL_Surface The scaled surface or NULL if error.
		 */
		SDL_Surface* sdlScaleSurface(SDL_Surface *src, double newWidth, double newHeight);
		
		/**
		 * Log a message to a log file.
		 * 
		 * @access public
		 * 
		 * @param char format Message format to log.
		 */
		void logMsg(const char *format, ...);
								
		/**
		 * Popup the standard PSP message dialog.
		 * 
		 * @access public
		 * 
		 * @param std::string msg Message to show.
		 * @param bool[optional] yesNoOptions Whether to display the yes and no options.
		 * 
		 * @return pspUtilityMsgDialogParams
		 */
		pspUtilityMsgDialogParams showPspMsgDialog(const std::string& msg, bool yesNoOptions = false);
		
		/**
		 * Popup the standard PSP error dialog.
		 * 
		 * @access public
		 * 
		 * @param int errorNum Error number.
		 * 
		 * @return pspUtilityMsgDialogParams
		 */
		pspUtilityMsgDialogParams showPspErrDialog(const unsigned int errorNum);
		
		/**
		 * Popup the standard PSP net dialog.
		 * 
		 * @access public
		 * 
		 * @return pspUtilityNetconfData
		 */
		pspUtilityNetconfData showPspNetDialog();
		
		/**
		 * Load a resource from a resource pak. (Basically a zip file)
		 * 
		 * @access public
		 * 
		 * @param std::string pakFile The Pak file.
		 * @param std::string resource Resource file.
		 * 
		 * @return std::string The resource.
		 */
		std::string loadResourceFromPak(const std::string& pakFile, const std::string& resource);
		
		/**
		 * Load a SDL Surface from an image in memory[RAW].
		 * 
		 * @access public
		 * 
		 * @param std::string buffer Data to load.
		 * 
		 * @return SDL_Surface* SDL_Surface.
		 */
		SDL_Surface* sdlSurfaceFromImageRaw(const std::string& buffer);
		
		/**
		 * Load a SDL Surface from an image in a file.
		 * 
		 * @access public
		 * 
		 * @param std::string file File to load.
		 * 
		 * @return SDL_Surface* SDL_Surface.
		 */
		SDL_Surface* sdlSurfaceFromImageFile(const std::string& file);
		
		/**
		 * Load a SDL Surface from an image in a pak file.
		 * 
		 * @access public
		 * 
		 * @param std::string pakFile The Pak file.
		 * @param std::string imageFile Image file.
		 * 
		 * @return SDL_Surface* SDL_Surface.
		 */
		SDL_Surface* sdlSurfaceFromPakFile(const std::string& pakFile, const std::string& imageFile);
		
		/**
		 * Load an OpenGL Texture from SDL Surface.
		 * 
		 * @access public
		 * 
		 * @param SDL_Surface* surface The SDL Surface.
		 * @param SDL_Rect*[optional] clip The portion to use.
		 * @param bool smooth Use GL_LINEAR.
		 * 
		 * @return xMTexture The texture + extra info.
		 */
		xMTexture glTextureFromSDLSurface(SDL_Surface *surface, SDL_Rect *clip = NULL, bool smooth = false);
		
		/**
		 * Render an OpenGL texture at desired coordinates.
		 * 
		 * @access public
		 * 
		 * @param int x X coordinate.
		 * @param int y Y coordinate.
		 * @param xMTexture texture The texture to render.
		 * @param bool smooth Use GL_LINEAR.
		 */
		void renderGlTexture(int x, int y, xMTexture texture, bool smooth = false);
		
		/**
		 * Open from memory[RAW].
		 * 
		 * @access public
		 * 
		 * @param std::string buffer Data to load.
		 * @oaram int size Font point size.
		 * 
		 * @return TTF_Font True Type Font.
		 */
		TTF_Font* ttfFontFromRaw(const std::string& buffer, int size);
		
		/**
		 * Open from font in pak file.
		 * 
		 * @access public
		 * 
		 * @param std::string pakFile The Pak file.
		 * @param std::string fontFile Font file.
		 * @oaram int size Font point size.
		 * 
		 * @return TTF_Font True Type Font.
		 */
		TTF_Font* ttfFontFromPak(const std::string& pakFile, const std::string& fontFile, int size);
		
		/**
		 * Get an OpenGL texture made from the text.
		 * 
		 * @access public
		 * 
		 * @param const char* text Text.
		 * @param TTF_Font* font The font to use.
		 * @param SDL_Color textColour Text colour.
		 * 
		 * @return xMTexture The texture.
		 */
		xMTexture ttfTexture(const char* text, TTF_Font *font, SDL_Color textColour);
		
		/**
		 * Get jostick number {which}.
		 * 
		 * @access public
		 * 
		 * @param int which Which Joystick to return.
		 * 
		 * @return SDL_Joystick Joystick.
		 */
		SDL_Joystick* getJoystick(int which);
		
		/**
		 * Play backgound music.
		 * 
		 * @access public
		 * 
		 * @param const std::string& file Music file.
		 * @param bool loop Loop music?
		 */
		void playBGMusic(const std::string& file, bool loop);
		
		/**
		 * Pause background music.
		 * 
		 * @access public
		 */
		void pauseBGMusic();

		/**
		 * Stop background music from playing.
		 * 
		 * @access public
		 */
		void stopBGMusic();
		
		/**
		 * Get current time.
		 * 
		 * @access public
		 * 
		 * @return std::string Time in 24 hr format. Hour:Minute
		 */
		std::string getTime();
		
		/**
		 * Get current date and time.
		 * 
		 * @access public
		 * 
		 * @return std::string DateTime
		 */
		std::string getDateTime();
		
};

#endif /* _LeXEngine_H */
