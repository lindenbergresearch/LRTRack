#!/bin/bash

RELEASE_FILE='src/Release.h'

# grep release information from Release.h
VERSION_MAJOR=$(cat $RELEASE_FILE | grep 'e VERSION_MAJOR' | cut -d" " -f3)
VERSION_MINOR=$(cat $RELEASE_FILE | grep 'e VERSION_MINOR' | cut -d" " -f3)
VERSION_PATCH=$(cat $RELEASE_FILE | grep 'e VERSION_PATCH' | cut -d" " -f3)
VERSION_STR="v"$VERSION_MAJOR"."$VERSION_MINOR"."$VERSION_PATCH

VERSION_NAME="LindenbergResearch-"$VERSION_STR"-VCVRack-v0.5.0"
TARGET_DIR=releases/$VERSION_STR
TARGET_FILE=releases/$VERSION_NAME".dmg"

mkdir -p $TARGET_DIR

cp -Rf ./res $TARGET_DIR
cp *.dylib $TARGET_DIR
cp LICENSE.txt $TARGET_DIR

hdiutil create -volname $VERSION_NAME -srcfolder $TARGET_DIR -ov -format UDZO $TARGET_FILE

# cleanup
rm -Rf $TARGET_DIR