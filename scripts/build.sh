#!/bin/bash
#
# this should be run from the src directory which contains the subsurface
# directory; the layout should look like this:
# .../src/subsurface
#
# the script will build Subsurface and libdivecomputer (plus some other
# dependencies if requestsed) from source.
#
# it installs the libraries and subsurface in the install-root subdirectory
# of the current directory (except on Mac where the Subsurface.app ends up
# in subsurface/build)
#
# by default it puts the build folders in
# ./subsurface/libdivecomputer/build (libdivecomputer build)
# ./subsurface/build                  (desktop build)
# ./subsurface/build-mobile           (mobile build)
#
# there is basic support for building from a shared directory, e.g., with
# one subsurface source tree on a host computer, accessed from multiple
# VMs as well as the host to build without stepping on each other - the
# one exceptioin is running autotools for libdiveconputer which has to
# happen in the shared libdivecomputer folder
# one way to achieve this is to have ./subsurface be a symlink; in that
# case the build directories are libdivecomputer/build, build, build-mobile
# alternatively a build prefix can be explicitly given with -build-prefix
# that build prefix is directly pre-pended to the destinations mentioned
# above - if this is a directory, it needs to end with '/'

# don't keep going if we run into an error
set -e

# create a log file of the build

exec 1> >(tee build.log) 2>&1

SRC=$(pwd)

if [[ -L subsurface && -d subsurface ]] ; then
	# ./subsurface is a symbolic link to the source directory, so let's
	# set up a prefix that puts the build directories in the current directory
	# but this can be overwritten via the command line
	BUILD_PREFIX="$SRC/"
fi

PLATFORM=$(uname)

BTSUPPORT="ON"
DEBUGRELEASE="Debug"
SRC_DIR="subsurface"

# deal with all the command line arguments
while [[ $# -gt 0 ]] ; do
	arg="$1"
	case $arg in
		-no-bt)
			# force Bluetooth support off
			BTSUPPORT="OFF"
			;;
		-quick)
			# only build libdivecomputer and Subsurface - this assumes that all other dependencies don't need rebuilding
			QUICK="1"
			;;
		-src-dir)
			# instead of using "subsurface" as source directory, use src/<srcdir>
			# this is convinient when using "git worktree" to have multiple branches
			# checked out on the computer.
			# remark <srcdir> must be in src (in parallel to subsurface), in order to
			# use the same 3rd party set.
			shift
			SRC_DIR="$1"
			;;			
		-build-deps)
			# in order to build the dependencies on Mac for release builds (to deal with the macosx-version-min for those
			# call this script with -build-deps
			BUILD_DEPS="1"
			;;
		-build-prefix)
			# instead of building in build & build-mobile in the current directory, build in <buildprefix>build
			# and <buildprefix>build-mobile; notice that there's no slash between the prefix and the two directory
			# names, so if the prefix is supposed to be a path, add the slash at the end of it, or do funky things
			# where build/build-mobile get appended to partial path name
			shift
			BUILD_PREFIX="$1"
			;;
		-build-with-webkit)
			# unless you build Qt from source (or at least webkit from source, you won't have webkit installed
			# -build-with-webkit tells the script that in fact we can assume that webkit is present (it usually
			# is still available on Linux distros)
			BUILD_WITH_WEBKIT="1"
			;;
		-mobile)
			# we are building Subsurface-mobile
			# Note that this will run natively on the host OS.
			# To cross build for Android or iOS (including simulator) 
			# use the scripts in packaging/xxx
			BUILD_MOBILE="1"
			;;
		-desktop)
			# we are building Subsurface
			BUILD_DESKTOP="1"
			;;
		-both)
			# we are building Subsurface and Subsurface-mobile
			BUILD_MOBILE="1"
			BUILD_DESKTOP="1"
			;;
		-create-appdir)
			# we are building an AppImage as by product
			CREATE_APPDIR="1"
			;;
		-release)
			# don't build Debug binaries
			DEBUGRELEASE="Release"
			;;
		*)
			echo "Unknown command line argument $arg"
			echo "Usage: build.sh [-no-bt] [-quick] [-build-deps] [-src-dir <SUBSURFACE directory>] [-build-prefix <PREFIX>] [-build-with-webkit] [-mobile] [-desktop] [-both] [-create-appdir] [-release]"
			exit 1
			;;
	esac
	shift
done

if [ "$BUILD_DEPS" = "1" ] && [ "$QUICK" = "1" ] ; then
	echo "Conflicting options; cannot request combine -build-deps and -quick"
	exit 1;
fi

# Verify that the Xcode Command Line Tools are installed
if [ "$PLATFORM" = Darwin ] ; then
	if [ -d /Developer/SDKs ] ; then
		SDKROOT=/Developer/SDKs
	elif [ -d /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs ] ; then
		SDKROOT=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs
	else
		echo "Cannot find SDK sysroot (usually /Developer/SDKs or"
		echo "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs)"
		exit 1;
	fi
	BASESDK=$(ls $SDKROOT | grep "MacOSX10\.1.\.sdk" | head -1 | sed -e "s/MacOSX//;s/\.sdk//")
	OLDER_MAC="-mmacosx-version-min=10.11 -isysroot${SDKROOT}/MacOSX${BASESDK}.sdk"
	OLDER_MAC_CMAKE="-DCMAKE_OSX_DEPLOYMENT_TARGET=${BASESDK} -DCMAKE_OSX_SYSROOT=${SDKROOT}/MacOSX${BASESDK}.sdk/"
	if [[ ! -d /usr/include && ! -d "${SDKROOT}/MacOSX.sdk/usr/include" ]] ; then
		echo "Error: Xcode Command Line Tools are not installed"
		echo ""
		echo "Please run:"
		echo " xcode-select --install"
		echo "to install them (you'll have to agree to Apple's licensing terms etc), then run build.sh again"
		exit 1;
	fi
fi

# normally this script builds the desktop version in subsurface/build
# if the first argument is "-mobile" then build Subsurface-mobile in "$BUILD_PREFIX"build-mobile
# if the first argument is "-both" then build both in subsurface/build and "$BUILD_PREFIX"build-mobile

if [ "$BUILD_MOBILE" = "1" ] ; then
	echo "building Subsurface-mobile in ${SRC_DIR}/build-mobile"
	BUILDS=( "MobileExecutable" )
	BUILDDIRS=( "${BUILD_PREFIX}build-mobile" )
else
	# if no options are given, build Subsurface
	BUILD_DESKTOP="1"
fi

if [ "$BUILD_DESKTOP" = "1" ] ; then
	echo "building Subsurface in ${SRC_DIR}/build"
	BUILDS+=( "DesktopExecutable" )
	BUILDDIRS+=( "${BUILD_PREFIX}build" )
	if [ "$BUILD_WITH_WEBKIT" = "1" ] ; then
		PRINTING="-DNO_PRINTING=OFF"
	else
		PRINTING="-DNO_PRINTING=ON"
	fi
fi

if [[ ! -d "${SRC_DIR}" ]] ; then
	echo "please start this script from the directory containing the Subsurface source directory"
	exit 1
fi

if [ -z "$BUILD_PREFIX" ] ; then
	INSTALL_ROOT=$SRC/install-root
else
	INSTALL_ROOT="$BUILD_PREFIX"install-root
fi
mkdir -p "$INSTALL_ROOT"
export INSTALL_ROOT

# make sure we find our own packages first (e.g., libgit2 only uses pkg_config to find libssh2)
export PKG_CONFIG_PATH=$INSTALL_ROOT/lib/pkgconfig:$PKG_CONFIG_PATH

echo Building from "$SRC", installing in "$INSTALL_ROOT"

# find qmake
if [ -n "$CMAKE_PREFIX_PATH" ] ; then
	QMAKE=$CMAKE_PREFIX_PATH/../../bin/qmake
else
	hash qmake > /dev/null 2> /dev/null && QMAKE=qmake
	[ -z $QMAKE ] && hash qmake-qt5 > /dev/null 2> /dev/null && QMAKE=qmake-qt5
	[ -z $QMAKE ] && echo "cannot find qmake or qmake-qt5" && exit 1
fi

# on Debian and Ubuntu based systems, the private QtLocation and
# QtPositioning headers aren't bundled. Download them if necessary.
if [ "$PLATFORM" = Linux ] ; then
	QT_HEADERS_PATH=$($QMAKE -query QT_INSTALL_HEADERS)
	QT_VERSION=$($QMAKE -query QT_VERSION)

	if [ ! -d "$QT_HEADERS_PATH/QtLocation/$QT_VERSION/QtLocation/private" ] &&
           [ ! -d "$INSTALL_ROOT"/include/QtLocation/private ] ; then
		echo "Missing private Qt headers for $QT_VERSION; downloading them..."

		QTLOC_GIT=./qtlocation_git
		QTLOC_PRIVATE=$INSTALL_ROOT/include/QtLocation/private
		QTPOS_PRIVATE=$INSTALL_ROOT/include/QtPositioning/private

		rm -rf $QTLOC_GIT > /dev/null 2>&1
		rm -rf "$INSTALL_ROOT"/include/QtLocation > /dev/null 2>&1
		rm -rf "$INSTALL_ROOT"/include/QtPositioning > /dev/null 2>&1

		git clone --branch "v$QT_VERSION" git://code.qt.io/qt/qtlocation.git --depth=1 $QTLOC_GIT

		mkdir -p "$QTLOC_PRIVATE"
		cd $QTLOC_GIT/src/location
		find . -name '*_p.h' -print0 | xargs -0 cp -t "$QTLOC_PRIVATE"
		cd "$SRC"

		mkdir -p "$QTPOS_PRIVATE"
		cd $QTLOC_GIT/src/positioning
		find . -name '*_p.h' -print0 | xargs -0 cp -t "$QTPOS_PRIVATE"
		cd "$SRC"

		echo "* cleanup..."
		rm -rf $QTLOC_GIT > /dev/null 2>&1
	fi
fi

# set up the right file name extensions
if [ "$PLATFORM" = Darwin ] ; then
	SH_LIB_EXT=dylib
	if [ ! "$BUILD_DEPS" == "1" ] ; then
		pkg-config --exists libgit2 && LIBGIT=$(pkg-config --modversion libgit2) && LIBGITMAJ=$(echo $LIBGIT | cut -d. -f1) && LIBGIT=$(echo $LIBGIT | cut -d. -f2)
		if [[ "$LIBGITMAJ" -gt "0" || "$LIBGIT" -gt "25" ]] ; then
			LIBGIT2_FROM_PKGCONFIG="-DLIBGIT2_FROM_PKGCONFIG=ON"
		fi
	fi
else
	SH_LIB_EXT=so

	LIBGIT_ARGS=" -DLIBGIT2_DYNAMIC=ON "
	# check if we need to build libgit2 (and do so if necessary)
	# first check pkgconfig (that will capture our own local build if
	# this script has been run before)
	if pkg-config --exists libgit2 ; then
		LIBGIT=$(pkg-config --modversion libgit2)
		LIBGITMAJ=$(echo $LIBGIT | cut -d. -f1)
		LIBGIT=$(echo $LIBGIT | cut -d. -f2)
		if [[ "$LIBGITMAJ" -gt "0" || "$LIBGIT" -gt "25" ]] ; then
			LIBGIT2_FROM_PKGCONFIG="-DLIBGIT2_FROM_PKGCONFIG=ON"
		fi
	fi
	if [[ "$LIBGITMAJ" -lt "1" && "$LIBGIT" -lt "26" ]] ; then
		# maybe there's a system version that's new enough?
		# Ugh that's uggly - read the ultimate filename, split at the last 'o' which gets us ".0.26.3" or ".1.0.0"
		# since that starts with a dot, the field numbers in the cut need to be one higher
		LIBGIT=$(realpath $(ldconfig -p | grep libgit2\\.so\\. | cut -d\  -f4) | awk -Fo '{ print $NF }')
		LIBGITMAJ=$(echo $LIBGIT | cut -d. -f2)
		LIBGIT=$(echo $LIBGIT | cut -d. -f3)
	fi
fi

if [[ $PLATFORM = Darwin && "$BUILD_DEPS" == "1" ]] ; then
	# when building distributable binaries on a Mac, we cannot rely on anything from Homebrew,
	# because that always requires the latest OS (how stupid is that - and they consider it a
	# feature). So we painfully need to build the dependencies ourselves.
	cd "$SRC"

	./${SRC_DIR}/scripts/get-dep-lib.sh single . libz
	pushd libz
	# no, don't install pkgconfig files in .../libs/share/pkgconf - that's just weird
	sed -i .bak 's/share\/pkgconfig/pkgconfig/' CMakeLists.txt
	mkdir -p build
	cd build
	cmake "$OLDER_MAC_CMAKE" -DCMAKE_BUILD_TYPE="$DEBUGRELEASE" \
		-DCMAKE_INSTALL_PREFIX="$INSTALL_ROOT" \
		..
	make -j4
	make install
	popd

	./${SRC_DIR}/scripts/get-dep-lib.sh single . libcurl
	pushd libcurl
	bash ./buildconf
	mkdir -p build
	cd build
	CFLAGS="$OLDER_MAC" ../configure --prefix="$INSTALL_ROOT" --with-darwinssl \
		--disable-tftp --disable-ftp --disable-ldap --disable-ldaps --disable-imap --disable-pop3 --disable-smtp --disable-gopher --disable-smb --disable-rtsp
	make -j4
	make install
	popd

	./${SRC_DIR}/scripts/get-dep-lib.sh single . openssl
	pushd openssl
	mkdir -p build
	cd build
	../Configure --prefix="$INSTALL_ROOT" --openssldir="$INSTALL_ROOT" "$OLDER_MAC" darwin64-x86_64-cc
	make depend
	# all the tests fail because the assume that openssl is already installed. Odd? Still thinks work
	make -j4 -k
	make -k install
	popd

	./${SRC_DIR}/scripts/get-dep-lib.sh single . libssh2
	pushd libssh2
	mkdir -p build
	cd build
	cmake "$OLDER_MAC_CMAKE" -DCMAKE_INSTALL_PREFIX="$INSTALL_ROOT" -DCMAKE_BUILD_TYPE=$DEBUGRELEASE -DBUILD_SHARED_LIBS=ON -DBUILD_TESTING=OFF -DBUILD_EXAMPLES=OFF ..
	make -j4
	make install
	popd
	if [ "$PLATFORM" = Darwin ] ; then
		# in order for macdeployqt to do its job correctly, we need the full path in the dylib ID
		cd "$INSTALL_ROOT"/lib
		NAME=$(otool -L libssh2.dylib | grep -v : | head -1 | cut -f1 -d\  | tr -d '\t')
		echo "$NAME" | if grep -v / > /dev/null 2>&1 ; then
			install_name_tool -id "$INSTALL_ROOT/lib/$NAME" "$INSTALL_ROOT/lib/$NAME"
		fi
	fi
fi

if [[ "$LIBGITMAJ" -lt "1" && "$LIBGIT" -lt "26" ]] ; then
	LIBGIT_ARGS=" -DLIBGIT2_INCLUDE_DIR=$INSTALL_ROOT/include -DLIBGIT2_LIBRARIES=$INSTALL_ROOT/lib/libgit2.$SH_LIB_EXT "

	cd "$SRC"

	./${SRC_DIR}/scripts/get-dep-lib.sh single . libgit2
	pushd libgit2
	mkdir -p build
	cd build
	cmake "$OLDER_MAC_CMAKE" -DCMAKE_INSTALL_PREFIX="$INSTALL_ROOT" -DCMAKE_BUILD_TYPE="$DEBUGRELEASE" -DBUILD_CLAR=OFF ..
	make -j4
	make install
	popd

	if [ "$PLATFORM" = Darwin ] ; then
		# in order for macdeployqt to do its job correctly, we need the full path in the dylib ID
		cd "$INSTALL_ROOT/lib"
		NAME=$(otool -L libgit2.dylib | grep -v : | head -1 | cut -f1 -d\  | tr -d '\t')
		echo "$NAME" | if grep -v / > /dev/null 2>&1 ; then
			install_name_tool -id "$INSTALL_ROOT/lib/$NAME" "$INSTALL_ROOT/lib/$NAME"
		fi
	fi
fi

if [[ $PLATFORM = Darwin && "$BUILD_DEPS" == "1" ]] ; then
	# when building distributable binaries on a Mac, we cannot rely on anything from Homebrew,
	# because that always requires the latest OS (how stupid is that - and they consider it a
	# feature). So we painfully need to build the dependencies ourselves.
	cd "$SRC"
	./${SRC_DIR}/scripts/get-dep-lib.sh single . libzip
	pushd libzip
	mkdir -p build
	cd build
	cmake "$OLDER_MAC_CMAKE" -DCMAKE_BUILD_TYPE="$DEBUGRELEASE" \
		-DCMAKE_INSTALL_PREFIX="$INSTALL_ROOT" \
		..
	make -j4
	make install
	popd

	./${SRC_DIR}/scripts/get-dep-lib.sh single . hidapi
	pushd hidapi
	# there is no good tag, so just build master
	bash ./bootstrap
	mkdir -p build
	cd build
	CFLAGS="$OLDER_MAC" ../configure --prefix="$INSTALL_ROOT"
	make -j4
	make install
	popd

	./${SRC_DIR}/scripts/get-dep-lib.sh single . libusb
	pushd libusb
	bash ./bootstrap.sh
	mkdir -p build
	cd build
	CFLAGS="$OLDER_MAC" ../configure --prefix="$INSTALL_ROOT" --disable-examples
	make -j4
	make install
	popd

	./${SRC_DIR}/scripts/get-dep-lib.sh single . libftdi1
	pushd libftdi1
	mkdir -p build
	cd build
	cmake "$OLDER_MAC_CMAKE" -DCMAKE_BUILD_TYPE="$DEBUGRELEASE" \
		-DCMAKE_INSTALL_PREFIX="$INSTALL_ROOT" \
		..
	make -j4
	make install
	popd
fi


cd "$SRC"

# build libdivecomputer

cd ${SRC_DIR}

if [ ! -d libdivecomputer/src ] ; then
	git submodule init
	git submodule update --recursive
fi

mkdir -p "${BUILD_PREFIX}libdivecomputer/build"
cd "${BUILD_PREFIX}libdivecomputer/build"

if [ ! -f "$SRC"/${SRC_DIR}/libdivecomputer/configure ] ; then
	# this is not a typo
	# in some scenarios it appears that autoreconf doesn't copy the
	# ltmain.sh file; running it twice, however, fixes that problem
	autoreconf --install "$SRC"/${SRC_DIR}/libdivecomputer
	autoreconf --install "$SRC"/${SRC_DIR}/libdivecomputer
fi
CFLAGS="$OLDER_MAC -I$INSTALL_ROOT/include $LIBDC_CFLAGS" "$SRC"/${SRC_DIR}/libdivecomputer/configure --prefix="$INSTALL_ROOT" --disable-examples
if [ "$PLATFORM" = Darwin ] ; then
	# remove some copmpiler options that aren't supported on Mac
	# otherwise the log gets very noisy
	for i in $(find . -name Makefile)
	do
		sed -i .bak 's/-Wrestrict//;s/-Wno-unused-but-set-variable//' "$i"
	done
	# it seems that on my Mac some of the configure tests for libdivecomputer
	# pass even though the feature tested for is actually missing
	# let's hack around that
	# touch config.status, recreate config.h and then disable HAVE_CLOCK_GETTIME
	# this seems to work so that the Makefile doesn't re-run the
	# configure process and overwrite all the changes we just made
	touch config.status
	make config.h
	grep CLOCK_GETTIME config.h
	sed -i .bak 's/^#define HAVE_CLOCK_GETTIME 1/#undef HAVE_CLOCK_GETTIME /' config.h
fi
make -j4
make install

if [ "$PLATFORM" = Darwin ] ; then
	if [ -z "$CMAKE_PREFIX_PATH" ] ; then
		libdir=$($QMAKE -query QT_INSTALL_LIBS)
		if [ $? -eq 0 ]; then
			export CMAKE_PREFIX_PATH=$libdir/cmake
		elif [ -d "$HOME/Qt/5.9.1" ] ; then
			export CMAKE_PREFIX_PATH=~/Qt/5.9.1/clang_64/lib/cmake
		elif [ -d "$HOME/Qt/5.9" ] ; then
			export CMAKE_PREFIX_PATH=~/Qt/5.9/clang_64/lib/cmake
		elif [ -d "$HOME/Qt/5.8" ] ; then
			export CMAKE_PREFIX_PATH=~/Qt/5.8/clang_64/lib/cmake
		elif [ -d "$HOME/Qt/5.7" ] ; then
			export CMAKE_PREFIX_PATH=~/Qt/5.7/clang_64/lib/cmake
		elif [ -d "$HOME/Qt/5.6" ] ; then
			export CMAKE_PREFIX_PATH=~/Qt/5.6/clang_64/lib/cmake
		elif [ -d "$HOME/Qt/5.5" ] ; then
			export CMAKE_PREFIX_PATH=~/Qt/5.5/clang_64/lib/cmake
		elif [ -d /usr/local/opt/qt5/lib ] ; then
			# Homebrew location for qt5 package
			export CMAKE_PREFIX_PATH=/usr/local/opt/qt5/lib/cmake
		else
			echo "cannot find Qt 5.5 or newer in ~/Qt"
			exit 1
		fi
	fi
fi

cd "$SRC"

if [ "$BUILD_WITH_WEBKIT" = "1" ]; then
	EXTRA_OPTS="-DNO_USERMANUAL=OFF"
else
	EXTRA_OPTS="-DNO_USERMANUAL=ON"
fi

if [ "$QUICK" != "1" ] ; then
	# build the googlemaps map plugin

	cd "$SRC"
	./${SRC_DIR}/scripts/get-dep-lib.sh single . googlemaps
	pushd googlemaps
	mkdir -p build
	mkdir -p J10build
	cd build
	$QMAKE "INCLUDEPATH=$INSTALL_ROOT/include" ../googlemaps.pro
	# on Travis the compiler doesn't support c++1z, yet qmake adds that flag;
	# since things compile fine with c++11, let's just hack that away
	# similarly, don't use -Wdata-time
	if [ "$TRAVIS" = "true" ] ; then
		mv Makefile Makefile.bak
		cat Makefile.bak | sed -e 's/std=c++1z/std=c++11/g ; s/-Wdate-time//' > Makefile
	fi
	make -j4
	make install
	popd
fi

# finally, build Subsurface

set -x

for (( i=0 ; i < ${#BUILDS[@]} ; i++ )) ; do
	SUBSURFACE_EXECUTABLE=${BUILDS[$i]}
	BUILDDIR=${BUILDDIRS[$i]}
	echo "build $SUBSURFACE_EXECUTABLE in $BUILDDIR"

	cd "$SRC"/${SRC_DIR}

	# pull the plasma-mobile components from upstream if building Subsurface-mobile
	if [ "$SUBSURFACE_EXECUTABLE" = "MobileExecutable" ] ; then
		bash ./scripts/mobilecomponents.sh
	fi

	mkdir -p "$BUILDDIR"
	cd "$BUILDDIR"
	export CMAKE_PREFIX_PATH="$INSTALL_ROOT/lib/cmake;${CMAKE_PREFIX_PATH}"
	cmake -DCMAKE_BUILD_TYPE="$DEBUGRELEASE" \
		-DSUBSURFACE_TARGET_EXECUTABLE="$SUBSURFACE_EXECUTABLE" \
		"$LIBGIT_ARGS" \
		-DLIBDIVECOMPUTER_INCLUDE_DIR="$INSTALL_ROOT"/include \
		-DLIBDIVECOMPUTER_LIBRARIES="$INSTALL_ROOT"/lib/libdivecomputer.a \
		-DCMAKE_PREFIX_PATH="$CMAKE_PREFIX_PATH" \
		-DBTSUPPORT="$BTSUPPORT" \
		-DCMAKE_INSTALL_PREFIX="$INSTALL_ROOT" \
		$LIBGIT2_FROM_PKGCONFIG \
		-DFORCE_LIBSSH=OFF \
		$PRINTING $EXTRA_OPTS \
		"$SRC"/${SRC_DIR}

	if [ "$PLATFORM" = Darwin ] ; then
		rm -rf Subsurface.app
		rm -rf Subsurface-mobile.app
	fi

	LIBRARY_PATH=$INSTALL_ROOT/lib make -j4
	LIBRARY_PATH=$INSTALL_ROOT/lib make install

	if [ "$CREATE_APPDIR" = "1" ] ; then
		# if we create an AppImage this makes gives us a sane starting point
		cd "$SRC"
		mkdir -p ./appdir
		mkdir -p appdir/usr/share/metainfo
		mkdir -p appdir/usr/share/icons/hicolor/256x256/apps
		cp -r ./install-root/* ./appdir/usr
		cp ${SRC_DIR}/appdata/subsurface.appdata.xml appdir/usr/share/metainfo/
		cp ${SRC_DIR}/icons/subsurface-icon.png appdir/usr/share/icons/hicolor/256x256/apps/
	fi
done
