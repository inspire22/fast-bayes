# coding: utf-8
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'fast_bayes/version'

Gem::Specification.new do |spec|
  spec.name          = "fast_bayes"
  spec.version       = FastBayes::VERSION
  spec.authors       = ["Chang Rajani"]
  spec.email         = ["chang.rajani@gmail.com"]

  spec.summary       = %q{A naive bayes text classification algorithm implementation using C++ built for Ruby}
  spec.description   = %q{A naive bayes text classification algorithm built in C++, with a Ruby interface using Rice}
  spec.homepage      = "https://github.com/Coolnesss/fast-bayes"
  spec.license       = "MIT"

  spec.files         = `git ls-files -z`.split("\x0").reject do |f|
    f.match(%r{^(test|spec|features)/})
  end
  spec.bindir        = "exe"
  spec.executables   = spec.files.grep(%r{^exe/}) { |f| File.basename(f) }
  spec.extensions = %w[ext/fast_bayes/extconf.rb]
  spec.require_paths = ["lib"]

  spec.add_runtime_dependency 'rice', '~> 2.1', '>= 2.1.0'
  spec.add_runtime_dependency 'fast-stemmer', '~> 1.0', '>= 1.0.0'

  spec.add_development_dependency "bundler"
  # spec.add_development_dependency "rake", "~> 10.0"
  spec.add_development_dependency "rake", ">= 13.0"
  spec.add_development_dependency "rspec", "~> 3.0"
  spec.add_development_dependency "rake-compiler",'~> 1.0', '>= 1.0.3'
end
