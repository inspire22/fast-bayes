require 'mkmf-rice'
require "rubygems"
rice_dir = Gem::Specification.find_by_name("rice").gem_dir
$INCFLAGS << " -I#{File.join(rice_dir, "ruby/lib/include")}"


$CXXFLAGS += " -std=c++11 "
$CXXFLAGS += " -Ofast "
$CXXFLAGS += " -g0 "
create_makefile('fast_bayes/fast_bayes')
