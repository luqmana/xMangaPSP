xMangaPSP
-----------

This is a native PSP client for reading manga from various sources. Currently only MangaStream is supported. OneManga api support is available but OneManga no longer serves manga images.

http://omp.leonex.co.cc/
For help: onemangapsp@leonex.co.cc

Credits
-------

The xMangaPSP client would have not existed wihtout the efforts of the following people.

Luqman A. (ChaoticX[SinZ]) — Creator
The guys at ##psp-programmin on freenode — They provided a lot of help with different aspects of the program.
Th3man / Baba — For providing a PSP and PSP go to test with.
PSPSDK Devs — For making the necessary tools.
Samoi, kgsws, etc — For providing/helping with the signing tool.
The Mangakas/Scanlators — For bringing us the manga.
MangaStream.com — For hosting the manga.

To Build
--------

xMangaPSP can be built on any platform which supports the PSPSDK and the required libraries. So far building has been tested on:
	- Windows XP [Minimalist PSPSDK]
	- Windows Vista [Minimalist PSPSDK]
	- Ubuntu [64-bit] [SVN PSPToolchain + libs]
	- Ubuntu [64-bit] [Minimalist PSPSDK]
	
For a onestop solution I recommend Minimalist PSPSDK which installs the PSPSDK and the required libraries plus some extra ones. It is also kept up to date.

The required libraries for xMangaPSP are as follows: [The required libraries themselves have required libraries but those are not included]
	- SDL
	- pspgl
	- SDL_gfx
	- SDL_Image
	- SDL_Mixer
	- SDL_TTF
	- zziplib
	- curl
	- TinyXML

To Sign
-------

To sign xMangaPSP so that it can run on Official firmware, you need to find a prxencrypter or something of the sort.

You can also download the modified build.mak for the pspsdk @ http://omp.leonex.co.cc/signingtool/build.mak and the source for psp-prxsign @ http://omp.leonex.co.cc/signingtool/psp-prxsign.zip

Once you've downloaded the two files:
    - Place build.mak in $PSPSDK/lib 
    - Extract psp-prxsign.zip and compile the code using the makefile provided. (You'll need the openssl dev stuff)
    - Once you successfully compiled the code, place the resulting psp-prxsign executable in $PSPDEV/bin
    - Make sure the xMangaPSP makefile has SIGN=1 and compile.
    - You now have a signed eboot!

License
-------

xMangaPSP is licensed under the GNU Public License v3 or later. For full license see `LICENSE.txt`.
