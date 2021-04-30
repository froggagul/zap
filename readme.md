/var/log/wtmp
/var/run/utmp
/var/log/lastlog

`zap -a username -t tty -d mmddyy`
- a 옵션: account (username)을 선택하여 삭제하도록 함
- t 옵션: 접속했던 terminal 선택하여 편집하도록 함
- i 옵션: 접속했던 IP 선택하여 편집하도록 함
- d 옵션: 년월일시를 지정하여 특정 시간대의 접속기록을 편집할 수 있도록 함)  

`zap -R -a username1 username2 -t tty1 tty2 -d ddmmyy1 ddmmyy2`
- R 옵션을 주어서 선택한 기록을 다른 내용으로 대체할 수 있도록 편집기능을 구현한다. (삭제가 아닌 변조 기능)
