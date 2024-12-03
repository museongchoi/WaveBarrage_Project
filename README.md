# WaveBarrage

LOL PVE / 벰서라이크 집중포화 모작 프로젝트

<br>

---

<br>

## 🗂️ **프로젝트 개요**

| **진행 기간** | 2024.11.04 ~ 2024.11.25 / 22일 |
|---------------|---------------------------------|
| **핵심 기술** | RPC, Variable Replication |
| **팀 규모**   | 팀 프로젝트 총 3명 / 개발자: 최무성, 서지호, 박교녕 |
| **개발 환경** | UnrealEngine 5.4 ver, C++, GitHub, Notion |
| **PPT 링크** | https://www.miricanvas.com/v/13wpdm3 |
| **YouTube 데모 영상** | https://youtu.be/R9QI2_x66TI |

<br>

---

<br>

## 📂 **개발 내용**

<div style="display: flex; justify-content: space-between; align-items: center;">
    <div style="text-align: center; flex: 1;">
        <h3>WBPrototype Ver01</h3>
        <img src="https://github.com/user-attachments/assets/779813b3-89fc-48fc-8fbf-2ca8bf804910" alt="WBPrototype Ver01" width="400">
    </div>
    <div style="text-align: center; flex: 1;">
        <h3>WBAlpha Ver02</h3>
        <img src="https://github.com/user-attachments/assets/32b6d79b-b728-4055-80e7-a2a74291c6aa" alt="WBAlpha Ver02" width="400">
    </div>
</div>

<br>

---

<br>

### **01. 네트워크 동기화 및 상태 관리**
- **PlayerStates** 배열은 각 플레이어의 상태 정보를 저장하며, 서버에서 복제(`Replicate`)되어 클라이언트에서 상태를 일관되게 유지합니다.
- **고유 PlayerID 시스템 추가**
  - 게임 시작 시 **`AssignPlayerID()`** 함수를 통해 각 플레이어에게 고유 ID를 할당.
  - `AWBGameState`에서 모든 플레이어의 체력을 서버에서 일괄적으로 관리.
  - `OnRep_PlayerStates`를 통해 클라이언트에서 UI 동기화.
  - 체력 자동 회복 `UpdateAllPlayerHPHell` 및 데미지 처리 `UpdatePlayerHP` 구현.
  - 서버에서 몬스터의 타겟팅 로직을 처리하고, 클라이언트에서 동기화.
   
<br>

---

<br>

### **02. OPP 활용 유지보수성 향상**
- **객체 지향적 접근**을 통해 재사용 가능한 기능을 클래스로 나누어 유지 보수성을 높였습니다.
<br>

```
WB Project
├ WBGameMode                ├ WBWeaponBase               ├ WBMonsterBase
├ WBGameState               │└ Weapon                    ├ WBMonsterGroup
├ WBPlayerState             │├ Exclusive Weapon          ├ WBMonsterProjectile
├ WBPlayerController        │└ Standard Weapon           ├ WBMonsterSpawnerComponent
├ WBPlayerBase              ├ WBProjectileBase           ├ WBFSMComponent
│└ Jinx                     │└ Projectile Classes        ├ FWBPlayerST
                            ├ WBItemBase                 │├ Player ID
                            │└ Exp                       │└ HP
                                                         ├ WBSpawnDataTable
                                                         │└ 몬스터 타입별 스폰 정보
                                                         └ WBWeaponDataTable
                                                           └ 각 무기의 레벨별 스탯 정보
```

<br>

---

<br>

### **03. 멀티 플레이어 네트워크 구현 (리슨 서버)**
- **리슨 서버**를 사용하여 클라이언트 간의 데이터 동기화와 서버-클라이언트 간 통신을 구현하였습니다.
- **RPC (Remote Procedure Call)** 를 활용하여 스킬 사용, 경험치 공유 등의 기능을 서버와 클라이언트 간에 
동기화했습니다.
- **Variable Replication**을 통해 캐릭터 상태, 경험치, 무기 등을 모든 클라이언트에 일관되게 **복제**하였습니다.
- **Lifetime Replication**을 사용해 PlayerID 및 EquippedWeapons 같은 중요한 변수들을 모든 클라이언트와 
**동기화**했습니다.

<br>

---

<br>

### **04. 경험치 공유 및 레벨업 시스템**
- **경험치 공유 시스템**
  - **기능**
    - 모든 플레이어가 동일한 경험치 풀을 공유.
    - 몬스터 처치 시 얻은 경험치는 `AddExp` 함수에서 통합적으로 관리.
 
  - **개발 방법**
    - `AWBGameMode`에서 Exp, MaxExp, Level을 관리.
    - `UpdateGameState`를 통해 서버-클라이언트 간 동기화.
    - `LevelUp`함수에서 플레이어별 레벨업 처리를 동기화.
  
- **레벨업 카드 선택** : 레벨업 시 무기와 패시브를 강화할 수 있는 카드 중 랜덤으로 3개가 나타나며, 플레이어는 카드 하나를 선택.
- 게임에서 공유하는 레벨이나 경험치는 **GameState**에서 관리.

<br>

---

<br>

### **05. 캐릭터 및 무기 시스템**
- **자동 조준 및 수동 조준**
  - **자동 조준(Auto Aim)** : `SweepMultiByChannel`을 사용하여 주변의 몬스터들을 탐지하고 가장 가까운 적을 자동으로 타겟팅.
  - **수동 조준 모드** : 마우스 커서의 위치를 기반으로 캐릭터가 회전하도록 설정.
- **무기의 회전 동기화**
  - `ServerSetOrientation` 공격 요청이 발생하면 클라이언트로부터 서버에게 공격 방향 전송.
  - 서버는 해당 방향으로 캐릭터를 회전시킨 후, `MulticastSetOrientation` 함수를 통해 모든 클라이언트에게 공격 방향 전달.
- **자원 절약 및 최적화** : 타이머 매니저를 사용하여 공격 주기를 관리하여 원하는 타이밍에만 연산이 수행되게 구현하였습니다.

**개선안** : 오토모드에서 가장 가까운 몬스터를 찾는 알고리즘보다 더 최적화된 알고리즘을 적용할 것입니다.
- **현재 알고리즘의 시간 복잡도** : `O(N)` (충돌 체크) + `O(M)` (가장 가까운 적 탐색)
- **최적화 방안** :
    1. **공간 분할 알고리즘 (사분 트리/옥트리)** : `O(log N)` 시간 복잡도로 가장 가까운 적 탐색 가능.
    2. **K-최근접 이웃 탐색 (KD-트리 사용)** : `O(log N)` 시간 복잡도로 탐색 가능.
    3. **거리 제곱 비교** : `O(M)`이지만, 거리 비교 연산에서 제곱근을 피해 **연산 비용 감소**.

<br>

---

<br>

### **06. 무기 시스템 및 데이터 테이블 관리**
- **기능**
  - 데이터 테이블을 사용해 무기 속성(데미지, 쿨타임, 스킬 지속 시간 등)을 관리.
  - C2S_SpawnWeapon 함수로 서버-클라이언트 간 무기 동기화.

- **개발 방법**
  - 데이터 테이블에서 무기 속성을 불러와 무기 생성 및 업그레이드.
  - AWBPlayerState에 무기 레벨 데이터를 저장해 상태 관리.


**문제점** : 캐릭터의 전용 무기가 1레벨에서 2레벨로 업그레이드될 때, 해당 무기의 스텟이 올바르게 적용되지 않는 문제가 발생하였습니다. 
**원인** : 일반 무기와 전용 무기를 동일한 객체로 취급하여 상속 구조에서 발생한 예외 처리 미비로 인해 발생한 것으로 예상됩니다.
**해결 방안** : 일반 무기와 전용 무기를 서로 다른 객체로 분리하여 상속 구조에서 발생하는 문제를 해결하였습니다.

<br>

---

<br>

### **07. 몬스터 타겟 설정(S2C)**
- 단일 몬스터는 (비헤이비어 트리보다) 간단하게 동작하는 유한상태기계 상태 패턴을 사용. > 그래서 뭐가 좋음?
- 몬스터가 타겟을 탐색 할 때는 인근 몬스터를 몬스터 그룹으로 묶고 그룹 단위로 가까운 플레이어를 설정합니다.
- 몬스터 그룹은 소유한 몬스터가 모두 죽으면 자신도 소멸하여 메모리를 절약 및 게임 모드 게임 스테이트를 통해 일정 시간마다 가까운 플레이어를 브로트캐스트 하는 것으로 몬스터 클래스는 TICK 에서 아무 함수를 호출하지 않아 많은 리소스를 절약할 수 있었습니다. > 하고싶은 말이 뭐죠??????

<br>

---

<br>



<br>

---

<br>
