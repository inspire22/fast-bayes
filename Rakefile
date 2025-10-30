require "bundler/gem_tasks"
require "rspec/core/rake_task"
require "rake/extensiontask"

RSpec::Core::RakeTask.new(:spec)

# task :default => :spec
task :default => :compile

Rake::ExtensionTask.new "fast_bayes" do |ext|
  ext.lib_dir = "lib/fast_bayes"
end
