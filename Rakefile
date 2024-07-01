desc "Run all tests with Ceedling"
task :test do
  sh "bundle exec ceedling clobber test:all gcov:all utils:gcov"
end

desc "Build/Compile with esphome"
task :build do
  sh '[ ! -f "lexis.ttf" ] && wget -L -O "lexis.ttf" "https://github.com/damianvila/font-lexis/raw/v1.1/fonts/lexis.ttf" || true'
  sh "esphome -s VERSION ${VERSION:=0.0.0} compile childrensclock.yaml"
end