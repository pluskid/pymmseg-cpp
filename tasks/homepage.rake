namespace :homepage do
  desc 'generate homepage'
  task :generate do
    sh "cd misc && gerbil -u html homepage.erb > homepage.html"
  end

  desc 'publish homepage to rubyforge'
  task :publish => :generate do
    remote_path = "rubyforge.org:/var/www/gforge-projects"
    sh "scp misc/homepage.html #{remote_path}/rmmseg-cpp/index.html"
  end
end

desc 'alias to homepage:generate'
task :homepage => 'homepage:generate'
