# WaveBarrage

LOL PVE / 벰서라이크 집중포화 모작 프로젝트

<br>

---

<br>

## 🗂️ **프로젝트 개요**

| **진행 기간** | 2024.11.04 ~ 2024.11.25 / 22일 |
|---------------|---------------------------------|
| **핵심 기술** | 리슨 서버, RPC, Variable Replication |
| **팀 규모**   | 팀 프로젝트 총 3명 / 개발자: 최무성, 서지호, 박교녕 |
| **개발 환경** | UnrealEngine 5.4 ver, C++, GitHub, Notion |

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

### **01. 고유 PlayerID 시스템 추가**
- `FWBPlayerST` 구조체를 통해 **플레이어 ID** 관리하며, 각 플레이어의 상태를 서버에서 일관되게 유지합니다.
- 게임 시작 시 **`AssignPlayerID()`** 함수를 통해 각 플레이어에게 고유 ID를 할당하며, 이 ID를 통해 게임 내 상태를 서버에서 관리합니다.

<br>

---

<br>

### **02. Blueprint 프로토타입 및 C++ 로 변환**
- 초기에는 **블루프린트**를 사용해 게임의 프로토타입을 빠르게 개발하였으며,
이후 **C++로 리팩토링**하여 코드의 성능과 최적화를 향상시켰습니다.
- **객체 지향적 접근**을 통해 재사용 가능한 기능을 클래스로 나누어 유지 보수성을 높였습니다.

**문제점**: 특정 무기가 1레벨 상황에서 해당 무기 카드 선택 시 효과가 적용되지 않는 문제가 발생.
- **해결 방안**: 상속 구조를 체계적으로 분석하고 설계를 문서화하여 문제 방지.

<br>

---

<br>

### **03. 멀티 플레이어 네트워크 구현 (리슨 서버)**
- **리슨 서버**를 사용하여 클라이언트 간의 데이터 동기화와 서버-클라이언트 간 통신을 구현하였습니다.
- **RPC (Remote Procedure Call)**를 활용하여 스킬 사용, 경험치 공유 등의 기능을 서버와 클라이언트 간에 정확히 동기화했습니다.
- **Variable Replication**을 통해 캐릭터 상태, 경험치, 무기 등을 모든 클라이언트에 일관되게 복제하였습니다.

<br>

---

<br>

### **04. 경험치 공유 및 레벨업 시스템**
- **경험치 공유 시스템**: 모든 플레이어가 경험치를 공유하여 레벨업하는 시스템을 구현했습니다.
- **레벨업 카드 선택**: 레벨업 시 무기와 패시브를 강화할 수 있는 카드 중 랜덤으로 3개가 나타나며, 플레이어는 그중 하나를 선택.
- 게임에서 공유하는 레벨이나 경험치는 **GameState**에서 관리.

<br>

---

<br>

### **05. 캐릭터 및 무기 시스템**
- **캐릭터 무기 상호작용**: 캐릭터는 시작 시 **Champion 전용 무기**를 자동으로 장착하며, 무기가 캐릭터 메쉬에 부착됩니다.
- **무기의 회전 동기화**: 캐릭터의 회전 방향에 맞춰 무기의 회전도 일치하도록 설정하여, 투사체 발사 시 올바른 방향으로 발사되도록 처리.

<br>

---

<br>

### **06. 자동 조준 및 수동 조준**
- **자동 조준(Auto Aim)**: `SweepMultiByChannel`을 사용하여 주변의 몬스터들을 탐지하고 가장 가까운 적을 타겟팅.
- **수동 조준 모드**: 마우스 커서의 위치를 기반으로 캐릭터가 회전하도록 설정.
- **회전 정보 동기화**: `ServerSetOrientation`과 **`MulticastSetOrientation`**을 통해 서버와 클라이언트 간 회전 정보를 동기화.

<br>

---

<br>

### **07. 몬스터 타겟 설정 통신 (S2C)**
- `S2C_MGSetTargetPlayer()`와 `S2C_MBSetTargetPlayer()` 함수는 몬스터 그룹과 개별 몬스터의 타겟 플레이어를 설정.
- 서버에서 몬스터의 타겟팅 로직을 처리하고, 클라이언트에서 동기화.

<br>

---

<br>

### **08. 네트워크 동기화 및 상태 관리**
- **PlayerStates** 배열은 각 플레이어의 상태 정보를 저장하며, 서버에서 복제(`Replicate`)되어 클라이언트에서 상태를 일관되게 유지합니다.
- 체력의 변경이나 PlayerID 할당과 같은 중요한 상태 변화는 모두 서버에서 처리하고 클라이언트에 동기화되도록 설계.

<br>

---

<br>

## 📝 성과

<br>

### 📂 PPT 링크
[프로젝트 발표자료 보러가기](https://www.miricanvas.com/v/13wpdm3)

<br>

### 🎥 YouTube 데모 영상
<div align="center">
    <a href="https://youtu.be/R9QI2_x66TI">
        <img src="https://github.com/user-attachments/assets/59944cda-748b-414f-8a8e-c5c72c07a366" alt="Video Label" width="400">
    </a>
</div>

