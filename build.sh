set -euo pipefail

bundle exec jekyll build

git worktree add master

(cd master; git rm -r .)

cp -a _site/. master

cd master
git add .

git commit -m "Build $(date -Iseconds)"
