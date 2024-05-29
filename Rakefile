desc "Run all tests with Ceedling"
task :test do
  sh "bundle exec ceedling test:all"
end

desc "Build/Compile with esphome"
task :build do
  sh "esphome compile childrensclock.yaml"
end