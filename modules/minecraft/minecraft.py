import urllib.request
import shutil
import threading
import subprocess
import errno
import os

import servant
import support

# a quick and dirty implementation of an example minecraft server module.
# this only serves as a test and should not be seriously used

serverdir  = 'mcserver'
serverfile = 'mcserver.jar'

class Installer(support.Category):
    def __init__(self):
        super().__init__("Installer", "installer")

    def install_async(self):
        dl_url = 'https://launcher.mojang.com/v1/objects/3737db93722a9e39eeada7c27e7aca28b144ffa7/server.jar'
        try: os.makedirs('mcserver')
        except OSError as e:
            if e.errno != errno.EEXIST: raise
        
        with urllib.request.urlopen(dl_url) as response, open(os.path.join(serverdir, serverfile), 'wb') as out_file:
            shutil.copyfileobj(response, out_file)
        
        with open(os.path.join(serverdir, 'eula.txt'), 'w') as f:
            f.write('eula=true')

    @support.capability('install mc server')
    def install(self):
        threading.Thread(target=self.install_async).start()
        return servant.Result(servant.Result.OK, "installing server... this could take some time")

class Interaction(support.Category):
    def __init__(self):
        super().__init__("Interaction", "interaction")
        self.proc = None

    @support.capability('run server')
    def start(self):
        try:
            self.proc = subprocess.Popen(
                ["java","-Xmx1024M", "-Xms1024M", "-jar", serverfile, "nogui"], 
                stdout=subprocess.PIPE,
                stdin=subprocess.PIPE, 
                cwd=serverdir
            )
        except FileNotFoundError as e:
            return servant.Result(
                servant.Result.ERROR, 
                "failed to start server because either java or the mc server is not installed. ex: " + str(e)
            )

        return servant.Result(servant.Result.OK, "started mc server :3")

    @support.capability('stop server')
    def stop(self):
        self.proc.stdin.write(b"stop\n")
        self.proc.stdin.flush()
        return servant.Result(servant.Result.OK, "stopped server")

    @support.capability('execute command', servant.Text('command to execute'))
    def execute(self, command):
        self.proc.stdin.write((command.value + "\n").encode())
        self.proc.stdin.flush()
        return servant.Result(servant.Result.OK, "executed command")

__module__.add(Installer())
__module__.add(Interaction())