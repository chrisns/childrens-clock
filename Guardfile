guard :rake, task: 'test' do
  watch(%r{^src/(.+)\.h$})
  watch(%r{^test/(.+)\.c$})
  watch('project.yml')
  watch('Rakefile')
end

guard :rake, task: 'build' do
  watch(%r{^src/(.+)\.h$})
  watch('childrensclock.yaml')
end