author__ = 'fsleeman'

from os import popen, getppid, environ, system
from os.path import isfile

result = popen("ps -p %d -o pgid" % getppid()).read().strip()
pinnacle_pid = result.split('\n')[1]

environ['PINNACLE_PID'] = pinnacle_pid
if isfile('/tmp/add_structures_' + pinnacle_pid + '.Script'):
    system('cp /tmp/add_structures_' + pinnacle_pid + '.Script /tmp/add_structures.Script')
else:
    system('cp ' + environ['STRUCTURENAMING'] + '/scripts/empty.Script /tmp/add_structures.Script')

