desc "Run all tests with Ceedling"
task :test do
  sh "bundle exec ceedling clobber test:all gcov:all utils:gcov"
end

desc "Build/Compile with esphome"
task :build do
  sh "esphome compile childrensclock.yaml"
end