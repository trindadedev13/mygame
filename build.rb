require 'fileutils'

def run(cmd)
   puts cmd
   system(cmd) or abort("Failed #{cmd}")
end

FileUtils.mkdir_p("build")

files = Dir.glob("src/**/*.c").join(" ")

args = [
   "#{files}",
   "-Iinclude/",
   "-lSDL3",
   "-lSDL3_ttf",
   "-lSDL3_image",
   "-fsanitize=address",
   "-g",
   "-std=c23",
   "-o build/main"
].join(" ")

run("gcc #{args}")

HOME = ENV["HOME"]
ENV["DISPLAY"] = ":0"

FileUtils.mkdir_p("#{HOME}/temp/c/x11_example/")
FileUtils.cp("build/main", "#{HOME}/temp/c/x11_example/main")

EXECUTABLE = "main"
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

exec_path = File.expand_path("~/temp/c/x11_example/#{EXECUTABLE}")
File.chmod(0755, exec_path)

# Executa o binário
system(exec_path)

# Limpeza final
cleanup(x11_pid)