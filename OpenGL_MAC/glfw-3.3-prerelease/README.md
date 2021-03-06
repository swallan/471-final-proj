# GLFW

[![Build status](https://travis-ci.org/glfw/glfw.svg?branch=master)](https://travis-ci.org/glfw/glfw)
[![Build status](https://ci.appveyor.com/api/projects/status/0kf0ct9831i5l6sp/branch/master?svg=true)](https://ci.appveyor.com/project/elmindreda/glfw)
[![Coverity Scan](https://scan.coverity.com/projects/4884/badge.svg)](https://scan.coverity.com/projects/glfw-glfw)

## Introduction

GLFW is an Open Source, multi-platform library for OpenGL, OpenGL ES and Vulkan
application development.  It provides a simple, platform-independent API for
creating windows, contexts and surfaces, reading input, handling events, etc.

GLFW is licensed under the [zlib/libpng
license](http://www.glfw.org/license.html).

The latest stable release is version 3.2.1.

See the [downloads](http://www.glfw.org/download.html) page for details and
files, or fetch the `latest` branch, which always points to the latest stable
release.  Each release starting with 3.0 also has a corresponding [annotated
tag](https://github.com/glfw/glfw/releases) with source and binary archives.
The [version history](http://www.glfw.org/changelog.html) lists all user-visible
changes for every release.

This is a development branch for version 3.3, which is _not yet described_.

If you are new to GLFW, you may find the
[tutorial](http://www.glfw.org/docs/latest/quick.html) for GLFW
3 useful.  If you have used GLFW 2 in the past, there is a
[transition guide](http://www.glfw.org/docs/latest/moving.html) for moving to
the GLFW 3 API.


## Compiling GLFW

GLFW itself requires only the headers and libraries for your window system.  It
does not need the headers for any context creation API (WGL, GLX, EGL, NSGL) or
rendering API (OpenGL, OpenGL ES, Vulkan) to enable support for them.

GLFW supports compilation on Windows with Visual C++ 2010 and later, MinGW and
MinGW-w64, on macOS with Clang and on Linux and other Unix-like systems with GCC
and Clang.  It will likely compile in other environments as well, but this is
not regularly tested.

There are also [pre-compiled Windows
binaries](http://www.glfw.org/download.html) available for all compilers
supported on that platform.

See the [compilation guide](http://www.glfw.org/docs/latest/compile.html) in the
documentation for more information.


## Using GLFW

See the [building application guide](http://www.glfw.org/docs/latest/build.html)
guide in the documentation for more information.


## System requirements

GLFW supports Windows XP and later, OS X 10.7 Lion and later, and Linux and
other Unix-like systems with the X Window System.  Experimental implementations
for the Wayland protocol and the Mir display server are available but not yet
officially supported.

See the [compatibility guide](http://www.glfw.org/docs/latest/compat.html)
in the documentation for more information.


## Dependencies

GLFW itself depends only on the headers and libraries for your window system.

The examples and test programs depend on a number of tiny libraries.  These are
located in the `deps/` directory.

 - [getopt\_port](https://github.com/kimgr/getopt_port/) for examples
   with command-line options
 - [TinyCThread](https://github.com/tinycthread/tinycthread) for threaded
   examples
 - An OpenGL 3.2 core loader generated by
   [glad](https://github.com/Dav1dde/glad) for examples using modern OpenGL
 - [linmath.h](https://github.com/datenwolf/linmath.h) for linear algebra in
   examples
 - [Nuklear](https://github.com/vurtun/nuklear) for test and example UI
 - [stb\_image\_write](https://github.com/nothings/stb) for writing images to disk
 - [Vulkan headers](https://www.khronos.org/registry/vulkan/) for Vulkan tests

The Vulkan example additionally requires the Vulkan SDK to be installed, or it
will not be included in the build.

The documentation is generated with [Doxygen](http://doxygen.org/).  If CMake
does not find Doxygen, the documentation will not be generated when you build.


## Reporting bugs

Bugs are reported to our [issue tracker](https://github.com/glfw/glfw/issues).
Please check the [contribution
guide](https://github.com/glfw/glfw/blob/master/.github/CONTRIBUTING.md) for
information on what to include when reporting a bug.


## Changelog

- Added `glfwGetKeyScancode` function that allows retrieving platform dependent
  scancodes for keys (#830)
- Added `glfwSetWindowMaximizeCallback` and `GLFWwindowmaximizefun` for
  receiving window maximization events (#778)
- Added `glfwSetWindowAttrib` function for changing window attributes (#537)
- Added headless [OSMesa](http://mesa3d.org/osmesa.html) backend (#281,#850)
- Added definition of `GLAPIENTRY` to public header
- Added macOS specific `GLFW_COCOA_RETINA_FRAMEBUFFER` window hint
- Added macOS specific `GLFW_COCOA_FRAME_AUTOSAVE` window hint (#195)
- Added `GLFW_INCLUDE_ES32` for including the OpenGL ES 3.2 header
- Removed `GLFW_USE_RETINA` compile-time option
- Bugfix: Calling `glfwMaximizeWindow` on a full screen window was not ignored
- Bugfix: `GLFW_INCLUDE_VULKAN` could not be combined with the corresponding
          OpenGL and OpenGL ES header macros
- Bugfix: `glfwGetInstanceProcAddress` returned `NULL` for
          `vkGetInstanceProcAddr` when `_GLFW_VULKAN_STATIC` was enabled
- [Win32] Bugfix: Undecorated windows could not be iconified by the user (#861)
- [Win32] Bugfix: Deadzone logic could underflow with some controllers (#910)
- [Win32] Bugfix: Bitness test in `FindVulkan.cmake` was VS specific (#928)
- [X11] Replaced `_GLFW_HAS_XF86VM` compile-time option with dynamic loading
- [X11] Bugfix: `glfwGetVideoMode` would segfault on Cygwin/X
- [Cocoa] Added support for Vulkan window surface creation via
          [MoltenVK](https://moltengl.com/moltenvk/) (#870)
- [Cocoa] Bugfix: Disabling window aspect ratio would assert (#852)
- [Cocoa] Bugfix: Window creation failed to set first responder (#876,#883)
- [Cocoa] Bugfix: Removed use of deprecated `CGDisplayIOServicePort` function
                  (#165,#192,#508,#511)
- [Cocoa] Bugfix: Disabled use of deprecated `CGDisplayModeCopyPixelEncoding`
                  function on macOS 10.12+
- [Cocoa] Bugfix: Running in AppSandbox would emit warnings (#816,#882)
- [Cocoa] Bugfix: Windows created after the first were not cascaded (#195)
- [EGL] Added support for `EGL_KHR_get_all_proc_addresses` (#871)
- [EGL] Bugfix: The test for `EGL_RGB_BUFFER` was invalid


## Contact

On [glfw.org](http://www.glfw.org/) you can find the latest version of GLFW, as
well as news, documentation and other information about the project.

If you have questions related to the use of GLFW, we have a
[forum](http://discourse.glfw.org/), and the `#glfw` IRC channel on
[Freenode](http://freenode.net/).

If you have a bug to report, a patch to submit or a feature you'd like to
request, please file it in the
[issue tracker](https://github.com/glfw/glfw/issues) on GitHub.

Finally, if you're interested in helping out with the development of GLFW or
porting it to your favorite platform, join us on the forum, GitHub or IRC.


## Acknowledgements

GLFW exists because people around the world donated their time and lent their
skills.

 - Bobyshev Alexander
 - artblanc
 - arturo
 - Matt Arsenault
 - Keith Bauer
 - John Bartholomew
 - Niklas Behrens
 - Niklas Bergstr??m
 - Doug Binks
 - blanco
 - Kyle Brenneman
 - Martin Capitanio
 - Chi-kwan Chan
 - Lambert Clara
 - Andrew Corrigan
 - Noel Cower
 - Jason Daly
 - Jarrod Davis
 - Olivier Delannoy
 - Paul R. Deppe
 - Michael Dickens
 - ?????????? ????????????????
 - Mario Dorn
 - Jonathan Dummer
 - Ralph Eastwood
 - Siavash Eliasi
 - Michael Fogleman
 - Gerald Franz
 - M??rio Freitas
 - GeO4d
 - Marcus Geelnard
 - Eloi Mar??n Gratac??s
 - Stefan Gustavson
 - Sylvain Hellegouarch
 - Matthew Henry
 - heromyth
 - Lucas Hinderberger
 - Paul Holden
 - Warren Hu
 - IntellectualKitty
 - Aaron Jacobs
 - Erik S. V. Jansson
 - Toni Jovanoski
 - Arseny Kapoulkine
 - Osman Keskin
 - Cameron King
 - Peter Knut
 - Christoph Kubisch
 - Eric Larson
 - Robin Leffmann
 - Glenn Lewis
 - Shane Liesegang
 - Eyal Lotem
 - ???????????? ??????????????
 - Martins Mozeiko
 - Tristam MacDonald
 - Hans Mackowiak
 - Zbigniew Mandziejewicz
 - Kyle McDonald
 - David Medlock
 - Bryce Mehring
 - Jonathan Mercier
 - Marcel Metz
 - Jonathan Miller
 - Kenneth Miller
 - Bruce Mitchener
 - Jack Moffitt
 - Jeff Molofee
 - Jon Morton
 - Pierre Moulon
 - Julian M??ller
 - Kamil Nowakowski
 - Ozzy
 - Andri P??lsson
 - Peoro
 - Braden Pellett
 - Arturo J. P??rez
 - Orson Peters
 - Emmanuel Gil Peyrot
 - Cyril Pichard
 - Pieroman
 - Philip Rideout
 - Jorge Rodriguez
 - Ed Ropple
 - Aleksey Rybalkin
 - Riku Salminen
 - Brandon Schaefer
 - Sebastian Schuberth
 - Matt Sealey
 - SephiRok
 - Steve Sexton
 - Systemcluster
 - Yoshiki Shibukawa
 - Dmitri Shuralyov
 - Daniel Skorupski
 - Bradley Smith
 - Patrick Snape
 - Julian Squires
 - Johannes Stein
 - Michael Stocker
 - Justin Stoecker
 - Elviss Strazdins
 - Nathan Sweet
 - TTK-Bandit
 - Sergey Tikhomirov
 - Arthur Tombs
 - Ioannis Tsakpinis
 - Samuli Tuomola
 - Matthew Turner
 - urraka
 - Elias Vanderstuyft
 - Jari Vetoniemi
 - Ricardo Vieira
 - Nicholas Vitovitch
 - Simon Voordouw
 - Torsten Walluhn
 - Patrick Walton
 - Xo Wang
 - Jay Weisskopf
 - Frank Wille
 - yuriks
 - Santi Zupancic
 - Jonas ??dahl
 - Lasse ????rni
 - All the unmentioned and anonymous contributors in the GLFW community, for bug
   reports, patches, feedback, testing and encouragement

