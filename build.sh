set -euo pipefail

bundle exec jekyll build

git worktree add master

(cd master; git rm -r .)

cp -a _site/. master

cd master
git add .

# https://github.com/orgs/community/discussions/26560#discussioncomment-3252339
git -c user.name='github-actions[bot]' \
    -c user.email='41898282+github-actions[bot]@users.noreply.github.com' \
    commit -m "Build $(date -Iseconds)"
