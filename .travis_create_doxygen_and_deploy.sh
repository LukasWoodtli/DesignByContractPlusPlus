#!/bin/bash

# This script is partly taken from: https://gist.github.com/vidavidorra/548ffbcdae99d752da02

# Exit with nonzero exit code if anything fails
set -e
##### Configure git.
# Set the push default to simple i.e. push only the current branch.
git config --global push.default simple
# Pretend to be an user called Travis CI.
git config  --global user.name "Lukas Woodtli (Travis CI)"
git config  --global user.email "woodtli.lukas@gmail.com"

pwd
mkdir doc
doxygen Doxyfile

cd doc
pwd

git init
git add . --all
git commit -m "Deploy code docs to GitHub Pages"

pwd
git push --force "https://${GH_REPO_TOKEN}@github.com/LukasWoodtli/DesignByContractPlusPlus" master:gh-pages
