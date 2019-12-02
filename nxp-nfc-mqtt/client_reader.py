from subprocess import Popen, PIPE
from time import sleep
from nbstreamreader import NonBlockingStreamReader as NBSR

# run the shell as a subprocess:
p = Popen(['explorenfc-basic', '-k'],
        stdin = PIPE, stdout = PIPE, stderr = PIPE, shell = False)
# wrap p.stdout with a NonBlockingStreamReader object:
nbsr = NBSR(p.stdout)
# issue command:
# get the output
while True:
    output = nbsr.readline(0.1)
    # 0.1 secs to let the shell output the result
    if not output:
       continue
    print output
