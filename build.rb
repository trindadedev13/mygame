require 'fileutils'

def run(cmd)
   puts cmd
   system(cmd) or abort("Failed #{cmd}")
end

ARGV.each do |arg|
  case arg
    when "--android", "-a"
      Dir.chdir("android") do
        run("chmod +x gradlew")
        run("./gradlew assembleRelease")
      end
      exit 0
  end
end

FileUtils.mkdir_p("build")

Dir.chdir("desktop") do
  run("cmake -B build .")
  run("cmake --build build")
end

HOME = ENV["HOME"]
ENV["DISPLAY"] = ":0"

FileUtils.mkdir_p("#{HOME}/temp/c/mygame/")
FileUtils.cp("desktop/build/mygame", "#{HOME}/temp/c/mygame/mygame")

EXECUTABLE = "mygame"
x11_pid = spawn("termux-x11 :0")

def cleanup(x11_pid)
  puts "Shutting down termux-x11..."
  Process.kill("TERM", x11_pid)
  exit
end

Signal.trap("INT") do
  cleanup(x11_pid)
end

sleep 5

exec_path = File.expand_path("~/temp/c/mygame/#{EXECUTABLE}")
File.chmod(0755, exec_path)

# Executa o binário
system(exec_path)

# Limpeza final
cleanup(x11_pid)