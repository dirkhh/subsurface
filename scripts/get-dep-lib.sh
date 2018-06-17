#!/bin/bash
#

# set version of 3rd party libraries
CURRENT_LIBZIP="1.2.0"
CURRENT_LIBGIT2="v0.26.0"
CURRENT_HIDAPI="hidapi-0.7.0"
CURRENT_LIBCURL="curl-7_54_1"
CURRENT_LIBUSB="v1.0.21"
CURRENT_OPENSSL="OpenSSL_1_1_0h"
CURRENT_LIBSSH2="libssh2-1.8.0"
CURRENT_XSLT="v1.1.29"
CURRENT_SQLITE="3190200"
CURRENT_LIBXML2="v2.9.4"
CURRENT_LIBFTDI="1.3"
CURRENT_KIRIGAMI="70c025ef6f6dc63c85180867f70f5e00ba5a8dba"
CURRENT_BREEZE_ICONS=""

# Checkout library from git
# Ensure specified version is checked out,
# while avoiding cloning/fetching if unnecessary.
#
# Arguments:
#	name - used as directory name
#	version - any tag/sha usable by git
#	url - repository url
#
git_checkout_library() {
	[ $# -ne 3 ] && return -1

	# for clarity
	local name=$1
	local version=$2
	local url=$3

	if [ ! -d "$name" ]; then
		git clone "$url" "$name"
	fi
	pushd "$name"

	local current_sha=$(git rev-parse HEAD)
	local target_sha=$(git rev-parse "$version")

	if [ ! "$current_sha" = "$target_sha" ] ; then
		git fetch origin
		if ! git checkout "$version" ; then
			echo "Can't find the right tag in $name - giving up"
			return -1
		fi
	fi
	popd
}


# deal with all the command line arguments
if [[ $# -ne 2 && $# -ne 3 ]] ; then
	echo "wrong number of parameters, format:"
	echo "get-dep-lib.sh <platform> <install dir>"
	echo "get-dep-lib.sh single <install dir> <lib>"
	echo "get-dep-lib.sh singleAndroid <install dir> <lib>"
	echo "where"
	echo "<platform> is one of scripts, ios or android"
	echo "(the name of the directory where build.sh resides)"
	echo "<install dir> is the directory to clone in"
	echo "<lib> is the name to be cloned"
	exit -1
fi

PLATFORM=$1
INSTDIR=$2
if [ ! -d ${INSTDIR} ] ; then
	echo "creating dir"
	mkdir -p ${INSTDIR}
fi

# FIX FOR ANDROID,
if [ "$PLATFORM" == "singleAndroid" ] ; then
	CURRENT_LIBZIP="1.1.3"
	CURRENT_OPENSSL="OpenSSL_1_0_2o"
fi
# no curl and old libs (never version breaks)
# check whether to use curl or wget
if [ "`which curl`" == "" ] ; then
	CURL="wget "
else
	CURL="curl -O "
fi
BUILD_COMMON="libzip libgit2 googlemaps"
case ${PLATFORM} in
	scripts)
		BUILD="${BUILD_COMMON} hidapi libcurl libusb openssl libssh2"
		;;
	ios)
		BUILD="${BUILD_COMMON} libxslt"
		;;
	android)
		BUILD="${BUILD_COMMON} libxslt sqlite libxml2 openssl libftdi libusb"
		;;
	single)
		BUILD="$3"
		;;
	singleAndroid)
		BUILD="$3"
		;;
	*)
		echo "Unknown platform ${PLATFORM}, choose between native, ios or android"
		;;
esac

# show what you are doing and stop when things break
set -x
set -e

# get ready to download needed sources
cd ${INSTDIR}

if [[ "$BUILD" = *"libcurl"* ]]; then
	git_checkout_library libcurl $CURRENT_LIBCURL https://github.com/curl/curl.git
fi

if [[ "$BUILD" = *"libgit2"* ]]; then
	git_checkout_library libgit2 $CURRENT_LIBGIT2 https://github.com/libgit2/libgit2.git
fi

if [[ "$BUILD" = *"libssh2"* ]]; then
	git_checkout_library libssh2 $CURRENT_LIBSSH2 https://github.com/libssh2/libssh2.git
fi

if [[ "$BUILD" = *"libusb"* ]]; then
	git_checkout_library libusb $CURRENT_LIBUSB https://github.com/libusb/libusb.git
fi

if [[ "$BUILD" = *"libxml2"* ]];then
	git_checkout_library libxml2 $CURRENT_LIBXML2 https://github.com/GNOME/libxml2.git
fi

if [[ "$BUILD" = *"libxslt"* ]]; then
	git_checkout_library libxslt $CURRENT_XSLT https://github.com/GNOME/libxslt.git
fi

if [[ "$BUILD" = *"breeze-icons"* ]]; then
	git_checkout_library breeze-icons master https://github.com/kde/breeze-icons.git
fi

if [[ "$BUILD" = *"googlemaps"* ]]; then
	git_checkout_library googlemaps master https://github.com/Subsurface-divelog/googlemaps.git
fi

if [[ "$BUILD" = *"hidapi"* ]]; then
	git_checkout_library hidapi master https://github.com/signal11/hidapi.git
fi

if [[ "$BUILD" = *"kirigami"* ]]; then
	git_checkout_library kirigami $CURRENT_KIRIGAMI https://github.com/KDE/kirigami.git
fi

if [[ "$BUILD" = *"openssl"* ]]; then
	git_checkout_library openssl $CURRENT_OPENSSL https://github.com/openssl/openssl.git
fi

if [[ "$BUILD" = *"libzip"* ]]; then
	if [[ ! -e libzip-${CURRENT_LIBZIP}.tar.xz || ! -d libzip ]]; then
		${CURL} https://subsurface-divelog.org/downloads/libzip-${CURRENT_LIBZIP}.tar.xz
		tar xJf libzip-${CURRENT_LIBZIP}.tar.xz
		mv libzip-${CURRENT_LIBZIP} libzip
	fi
fi

if [[ "$BUILD" = *"libftdi"* ]]; then
	if [[ -e libftdi1-${CURRENT_LIBFTDI}.tar.bz2 || ! -d libftdi1 ]]; then
		${CURL} https://www.intra2net.com/en/developer/libftdi/download/libftdi1-${CURRENT_LIBFTDI}.tar.bz2
		tar -jxf libftdi1-${CURRENT_LIBFTDI}.tar.bz2
		mv libftdi1-${CURRENT_LIBFTDI} libftdi1
	fi
fi

if [[ "$BUILD" = *"sqlite"* ]]; then
	if [[ ! -e sqlite-autoconf-${CURRENT_SQLITE}.tar.gz || ! -d sqlite ]]; then
		${CURL} http://www.sqlite.org/2017/sqlite-autoconf-${CURRENT_SQLITE}.tar.gz
		tar -zxf sqlite-autoconf-${CURRENT_SQLITE}.tar.gz
		mv sqlite-autoconf-${CURRENT_SQLITE} sqlite
	fi
fi
