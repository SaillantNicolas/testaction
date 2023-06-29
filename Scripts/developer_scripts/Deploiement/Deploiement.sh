#!/bin/bash
# TODO
# license.h CUSTOMER_NAME Attention espace dans le nom du client

CGAL_ROOT=$PWD
CGAL_GITREPO=""
CGAL_PURCHASED_VERSION=""
CGAL_VERSION=""
CGAL_RELEASE_COMMIT=""
CGAL_PACKAGE=()
CUSTOMER_NAME=""
CGAL_CHECK()
{
  echo "Final Check"
  filename=$1
  while read -r line; do
    if [[ "$line" == '#define CGAL_LICENSE_WARNING'* ]] || [[ "$line" == '#define CGAL_LICENSE_ERROR'* ]]; then
      echo "SET : $line" | awk '{print $1,$2,$4}'
    elif [[ "$line" == '//#define CGAL_LICENSE_WARNING'* ]] || [[ "$line" == '//#define CGAL_LICENSE_ERROR'* ]]; then
      echo "NOT SET : $line" | awk '{print $1,$2,$3,$5}'
    fi
  done < $filename
  echo " ----------------------- "
}
# Function to clone CGAL repo
CGAL_CLONE()
{
  cd repo
  if [[ ! -d "cgal" ]]; then
    echo "Cloning CGAL repo"
    git clone https://github.com/CGAL/cgal.git
  else
    echo "CGAL repo already cloned"
  fi
  CGAL_GITREPO=$PWD/cgal
  echo "Path to CGAL repo is $CGAL_GITREPO"
  cd ..
  echo " ----------------------- "
}
# Function to get CGAL version
CGAL_GET_VERSION()
{
  echo "Getting CGAL version from $1"
  CGAL_PURCHASED_VERSION=$(grep "CGAL_VERSION" $1 | awk '{print $3}')
  CGAL_MAJOR_RELEASE=${CGAL_PURCHASED_VERSION:1:2}
  CGAL_MINOR_RELEASE=${CGAL_PURCHASED_VERSION:3:2}
  CGAL_BUGFIX_RELEASE=${CGAL_PURCHASED_VERSION:5:1}
  CGAL_MAJOR_RELEASE=${CGAL_MAJOR_RELEASE#0}
  CGAL_MINOR_RELEASE=${CGAL_MINOR_RELEASE#0}
  if [ "$CGAL_BUGFIX_RELEASE" == "0" ]; then
    CGAL_VERSION="$CGAL_MAJOR_RELEASE.$CGAL_MINOR_RELEASE"
  else
    CGAL_VERSION="$CGAL_MAJOR_RELEASE.$CGAL_MINOR_RELEASE.$CGAL_BUGFIX_RELEASE"
  fi
  echo "-- CGAL version is $CGAL_VERSION"
  echo " ----------------------- "
}
# Function to get CGAL commit
CGAL_GET_COMMIT()
{
  CGAL_RELEASE_COMMIT=$(curl --silent https://api.github.com/repos/CGAL/cgal/tags | yq '.[] | select(.name == "v'$CGAL_VERSION'").commit.sha')
  echo "-- CGAL release commit is $CGAL_RELEASE_COMMIT"
  echo " ----------------------- "
}
# Function to create partial release
CGAL_PARTIAL_RELEASE()
{
  rsync -a --exclude='developer_scripts' --exclude='data' --exclude='demo' --exclude='test' --exclude='examples' --exclude='include/CGAL/*' --include='include/CGAL' --exclude='package_info/*' --include='package_info' "Release/CGAL-$CGAL_VERSION/" "RESULT/CGAL-$CGAL_VERSION"
  cd Release/CGAL-$CGAL_VERSION/include/CGAL
  cp config.h "$CGAL_ROOT/RESULT/CGAL-$CGAL_VERSION/include/CGAL/config.h"
  file=$(find . -type f | sed 's|^\./||')
  for pkg in "${CGAL_PACKAGE[@]}"
  do
    if [[ $pkg == "POLYGON_MESH_PROCESSING_"* ]];then
      cp -rf ../../package_info/Polygon_mesh_processing $CGAL_ROOT/RESULT/CGAL-$CGAL_VERSION/package_info/Polygon_mesh_processing
      cd license/Polygon_mesh_processing
      pmp=$(find . -type f | sed 's|^\./||')
      for f in $pmp
      do
        if grep -iq "$pkg\_" $f; then
          pkg=$(echo "Polygon_mesh_processing/$f" | sed 's|\.h||')
        fi
      done
      cd ../../
    else
      directory=$(find . -iname "$pkg" -type d | sed 's|^\./||')
      cp -rf ../../package_info/$directory $CGAL_ROOT/RESULT/CGAL-$CGAL_VERSION/package_info/$directory
    fi
    echo $pkg
    for f in $file
    do
      if grep -iq "#include <CGAL/license/$pkg.h>" $f; then
        parent_dir=$(dirname "$f")
        dest_dir="$CGAL_ROOT/RESULT/CGAL-$CGAL_VERSION/include/CGAL/$parent_dir"
        mkdir -p "$dest_dir"
        cp "$f" "$dest_dir"
      fi
    done
  done
  for f in $file
  do
    if ! grep -iq "#include <CGAL/license/ *" $f; then
      parent_dir=$(dirname "$f")
      dest_dir="$CGAL_ROOT/RESULT/CGAL-$CGAL_VERSION/include/CGAL/$parent_dir"
      mkdir -p "$dest_dir"
      cp "$f" "$dest_dir"
    fi
  done
  cd $CGAL_ROOT
  cp $1 RESULT/CGAL-$CGAL_VERSION/include/CGAL/license.h
  echo " ----------------------- "
}
# Function to create full release
CGAL_FULL_RELEASE()
{
  echo -e "FULL_RELEASE"
  cp -rf Release/CGAL-$CGAL_VERSION RESULT/CGAL-$CGAL_VERSION
  rm RESULT/CGAL-$CGAL_VERSION/include/CGAL/license.h
  cp $1 RESULT/CGAL-$CGAL_VERSION/include/CGAL/license.h
  sed -i "s,\$URL,\$URL https://github.com/CGAL/cgal/tree/v$CGAL_VERSION ,g" RESULT/CGAL-$CGAL_VERSION/include/CGAL/license.h
}
if [[ $# -eq 0 ]]; then
  echo "No arguments provided"
  exit 1
elif [[ ! -f "$1" ]]; then
  echo "$1 does not exist."
  exit 1
fi
CGAL_GET_VERSION $1
CGAL_GET_COMMIT
# Create directories
if [[ ! -d "Release" ]]; then
  mkdir Release
fi
if [[ ! -d "RESULT" ]]; then
  mkdir RESULT
fi
if [[ ! -d "repo" ]]; then
  mkdir repo
fi
# Clone CGAL repo
CGAL_CLONE
# Create CGAL release
if [[ ! -d "Release/CGAL-$CGAL_VERSION" ]]; then
  echo "Creating CGAL release"
  cmake -DGIT_REPO=$CGAL_GITREPO -DDESTINATION=Release -DCGAL_VERSION=$CGAL_VERSION -DCGAL_VERSION_NR=$CGAL_PURCHASED_VERSION -DVERBOSE="ON" -P $CGAL_GITREPO/Scripts/developer_scripts/cgal_create_release_with_cmake.cmake
else
  echo "CGAL release already created"
fi
# Get Release type
CGAL_RELEASE_TYPE=$(grep "TARBALL_TYPE" $1 | awk '{print $3}')
CUSTOMER_NAME=$(grep "CUSTOMER_NAME" $1 | awk '{print $3}')
# Get CGAL packages
while read -r line; do
  if [[ "$line" == *CGAL_*_LICENSE* ]]; then
    CGAL_PACKAGE+=("$(echo "$line" | sed -e 's/CGAL_//' -e 's/_COMMERCIAL//' -e 's/_LICENSE//' | awk '{print $2}')")
  fi
done < "$1"
echo " ----------------------- "
echo "$CGAL_RELEASE_TYPE"
echo "if [[ $CGAL_RELEASE_TYPE == "PARTIAL_RELEASE" ]]; then"
if [[ "$CGAL_RELEASE_TYPE" == "PARTIAL_RELEASE" ]]; then
  CGAL_PARTIAL_RELEASE $1
elif [[ "$CGAL_RELEASE_TYPE" == "FULL_RELEASE" ]]; then
  CGAL_FULL_RELEASE $1
else
  echo "TARBALL_TYPE not valid."
fi
echo "taring RESULT/"
tar -czf RESULT/cgal-$CGAL_VERSION.tar.gz RESULT/CGAL-$CGAL_VERSION
echo " ----------------------- "
#CGAL_CHECK RESULT/CGAL-$CGAL_VERSION/include/CGAL/license.h



#echo "Cleaning"
#echo -e "\t-- removing RESULT/CGAL-$CGAL_VERSION"
#rm -rf RESULT/CGAL-$CGAL_VERSION
echo "Pushing to CodeBasehq"
cd RESULT/CGAL-$CGAL_VERSION
#git init > /dev/null 2>&1
echo $CUSTOMER_NAME
if [[ ! `git branch --list CGAL-$CGAL_VERSION` ]]; then
  echo "Creating branch"
  git checkout -b CGAL-$CGAL_VERSION
else
  echo "Branch already exists"
  git switch CGAL-$CGAL_VERSION
fi
git add . > /dev/null 2>&1
git commit -m "CGAL-$CGAL_VERSION" > /dev/null 2>&1
git remote add codebase git@codebasehq.com:geometryfactory/$CUSTOMER_NAME/deployment.git
#git push codebase CGAL-$CGAL_VERSION
git remote remove codebase
#git push --set-upstream codebase CGAL-$CGAL_VERSION
git switch Deploiement