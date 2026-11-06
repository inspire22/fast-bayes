require 'mkmf'

$CXXFLAGS += " -std=c++11 -Ofast -g0 "
create_makefile('fast_bayes/fast_bayes')
