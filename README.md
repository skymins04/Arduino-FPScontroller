# Arduino-FPScontroller
아두이노를 이용한 FPS 컨트롤러 v0.1

```sh
아카데미과학 사의 K2 BB탄 소총을 기반으로 제작되었습니다.

FPScontrollerRX <- 수신부(아두이노 레오나르도 + HC-06[ROLE:M])
FPScontrollerTX <- 송신부(아두이노 프로 미니 + 각종 버튼 및 조이스틱 및 자이로 + HC-06[ROLE:S])

HC-06간 바인딩이 성립되기 위해선 서로 ROLE이 달라야하고 비번은 동일해야합니다.
role 변경 AT 명령 => AT+ROLE=(mode, M = master, S = slave)
pin(비번) 변경 AT 명령 => AT+PIN(password)

결선법은 pin.txt를 참고하세요.
```
