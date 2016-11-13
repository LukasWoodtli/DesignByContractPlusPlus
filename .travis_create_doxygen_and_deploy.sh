#!/bin/bash

# This script is partly taken from: https://gist.github.com/vidavidorra/548ffbcdae99d752da02

# Exit with nonzero exit code if anything fails
set -e

mkdir doc
cd doc

git clone -b gh-pages https://github.com/LukasWoodtli/DesignByContractPlusPlus.git .

##### Configure git.
# Set the push default to simple i.e. push only the current branch.
git config --global push.default simple
# Pretend to be an user called Travis CI.
git config user.name "Travis CI"
git config user.email "travis@travis-ci.org"

rm -rf *
echo "" > .nojekyll


cd ..
doxygen Doxyfile  2>&1 | tee doxygen.log

cd doc
git add --all
git commit -m "Deploy code docs to GitHub Pages"

git push --force --quiet "https://${GH_REPO_TOKEN}@github.com/LukasWoodtli/DesignByContractPlusPlus" master:gh-pages > /dev/null 2>&1
