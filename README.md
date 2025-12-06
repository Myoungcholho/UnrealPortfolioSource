# 📘언리얼 3D 액션 게임 프로젝트
<br>
<p align="center">
  <img
    width="800"
    src="https://github.com/user-attachments/assets/19a7d879-6a57-4b4d-961b-5254333bce36"
    alt="unreal" />
</p>

{프로젝트 내용 소개}

| 항목 | 내용 |
|------|------|
| 📹 소개 영상 | [📎포트폴리오 영상](https://www.youtube.com/watch?v=xfAdjhyRtIA) |
| 🕒 개발 기간 | 2025.01.06 ~ 2025.04.02 (87일) |
| 👤 개발 인원 | 1명 |
| 🧰 개발 환경 | C++ |
| 🛠 실행 및 디버깅 툴 | Unreal Engine 5 |

</br>

# 📘목차
- [구현 요약 내용](#구현-요약-내용-목차-이동)
- [핵심 주요 코드](#핵심-주요-코드-목차-이동)
- [문제 해결 경험(트러블 슈팅)](#문제-해결-경험트러블-슈팅-목차-이동)
- [프로젝트에서 얻은 것](#프로젝트에서-얻은-것-목차-이동)
- [개발 계기](#개발-계기-목차-이동)
- [구현 상세 내용](#구현-상세-내용-목차-이동)

</br>

# 📘구현 요약 내용 [(목차 이동)](#목차)

| 상세 설명 링크 | 구현 요약 |
|----------------------|------------------|
| [🧱 Core Architecture](#core) |  |
| [🌍 World / Object](#world) | |
| [🎨 Rendering](#rendering) |  |
| [🕺 Animation](#animation) |  |
| [📦 Asset](#asset) | |
| [🛠 Editor](#editor) |  |
| [📊 Profiling](#profiling) |  |

</br>

# 📘핵심 주요 코드 [(목차 이동)](#목차)

| 코드 파일 | 코드 설명 |
|----------|-----------|
| 파일명 [.h]() / [.cpp]() | 내용 |

</br>

# 📘문제 해결 경험(트러블 슈팅) [(목차 이동)](#목차)

<table style="border-collapse:collapse;">
  <tr>
    <th width="350" style="border:2px solid #ffb3b3; background:#ffe1e1;">
      📂 빠른 공격 모션에서 타격 누락 문제 (Hit Detection 안정화)
    </th>
    <th width="350" style="border:2px solid #ffd27f; background:#fff1d6;">
      📚 암살 몽타주 관리 구조 문제 → 세트 기반 구조로 리팩토링
    </th>
  </tr>

  <tr>
    <td width="350" style="border:2px solid #ffb3b3; background:#ffe1e1; vertical-align:top;">
      빠른 공격에서 발생하던 미적중 문제를 하이브리드 콜리전 체크 방식으로 해결했습니다.
      <br><br>
      <a href="#t0">[상세설명]</a>
    </td>
    <td width="350" style="border:2px solid #ffd27f; background:#fff1d6; vertical-align:top;">
      처형에서 세트로 따로 관리하던 구조를 단일 세트로 리팩토링해 유지보수 문제를 해결했습니다.
      <br><br>
      <a href="#t1">[상세설명]</a>
    </td>
  </tr>
</table>

<br>

<table style="border-collapse:collapse;">
  <tr>
    <th width="350" style="border:2px solid #a8ddff; background:#e6f6ff;">
      🧾 Delegate / GC Crash 문제 (UPROPERTY 관리 문제) (BehaviorTree / AI Flow 개선)
    </th>
    <th width="350" style="border:2px solid #a8ddff; background:#e6f6ff;">
      📘 과도한 FSM 의존으로 인한 상태 관리 문제 (BehaviorTree / AI Flow 개선)
    </th>
  </tr>

  <tr>
    <td width="350" style="border:2px solid #a8ddff; background:#e6f6ff; vertical-align:top;">
      60초 이후 주기적으로 발생하던 GC 기반 크래시 문제를 원인 분석 후 해결했습니다.
      <br><br>
      <a href="#t3">[상세설명]</a>
    </td>
    <td width="350" style="border:2px solid #a8ddff; background:#e6f6ff; vertical-align:top;">
      과도한 FSM 사용으로 코드 작성이 어려웠던 문제를 로직을 FSM에서 분리해 의존도를 줄이는 방식으로 해결했습니다.
      <br><br>
      <a href="#t3">[상세설명]</a>
    </td>
  </tr>
</table>

---

## 1. 빠른 공격 모션에서 타격 누락 문제 (Hit Detection 안정화) <a id="t0"></a> [(트러블 슈팅 목록 이동)](#문제-해결-경험트러블-슈팅-목차-이동)

<table>
  <tr>
    <td style="border:2px solid #4fa3ff; border-radius:8px; padding:12px 16px; background:#050812;">
      <strong>🧩 문제</strong>
      <ul>
        <li> 공격이 맞는 것처럼 보이는데 실제 게임 판정에서는 가끔 안 맞는 상황이 발생</li>
      </ul>
      <strong>🔍 원인 분석</strong></br>
      <img width="250" height="160" alt="image" src="https://github.com/user-attachments/assets/7be33959-d84e-4313-ae1a-ac8ae9719cd4" />
      <ul>
        <li> 프레임 사이 이동량이 커져 Collision 활성 구간을 건너뛰며 판정이 누락되는 문제 </li>
      </ul>
      <strong>🛠 해결</strong><br>
      <img width="250" height="160" alt="image" src="https://github.com/user-attachments/assets/9aa7b46b-4edb-487a-afac-7e0576136a10" />
      <ul>
        <li> Collision 단일 판정 방식을, Collision + Trace를 병행하는 Hybrid 방식으로 확장함 </li>
        <li> 이전 위치 ~ 현재 위치를 LineTrace로 따라가며 Collision이 놓친 대상도 추가 타격되도록 함 </li>
      </ul>
      <strong>✅ 결과</strong><br>
      <ul>
        <li> 빠른 공격에서 발생하던 타격 누락을 없애고, 프레임 의존적인 판정을 궤적 기반으로 바꿔 안정적인 히트 판정을 확보 </li>
      </ul>
      <strong>📚 배운 점</strong>
      <ul>
        <li> 콘텐츠 프로그래머는 도구를 폭넓게 이해하고 과감히 조합·응용하는 역할임을 배움 </li>
        <li> 단일 기능만으로 모든 상황을 해결할 수 없다는 점을 깨달음 </li>
      </ul>
    </td>
  </tr>
</table>

---

## 2. 암살 몽타주 관리 구조 문제 → 세트 기반 구조로 리팩토링 <a id="t1"></a> [(트러블 슈팅 목록 이동)](#문제-해결-경험트러블-슈팅-목차-이동)

<table>
  <tr>
    <td style="border:2px solid #ffd27f; border-radius:8px; padding:12px 16px; background:#120d05;">
      <strong>🧩 문제</strong>
      <ul>
        <li> 피격 모션은 피격자 쪽에서만 몽타주를 가지는 구조였음 </li>
        <li> 처형 연출에서는 공격자·피격자가 항상 같은 몽타주 세트를 써야 하지만 각각 따로 세팅해야 해서 누락·불일치 등 세팅 실수를 유발함 </li>
      </ul>
      <strong>🔍 원인 분석</strong>
      <ul>
        <li> 암살/처형처럼 두 캐릭터가 동시에 사용하는 몽타주를 ‘한 세트’로 관리하지 않고, 기존 피격처럼 각자 따로 들고 있도록 설계한 것이 근본 원인이었음 </li>
      </ul>
      <strong>🛠 해결</strong><br>
      <img width="250" height="160" alt="image" src="https://github.com/user-attachments/assets/132c0a45-7e2d-437a-8f5b-af033eaffb02" />
      <ul>
        <li> 피격 모션과 분리해, 처형 연출은 단일 ‘세트 데이터’ 기반 구조로 리팩토링 </li>
        <li> 암살 요청 시 중앙에서 적절한 세트를 선택해 각 캐릭터에 전달하는 방식으로 변경 </li>
      </ul>
      <strong>✅ 결과</strong>
      <ul>
        <li> 처형 데이터를 하나의 세트로 관리해 세팅 실수 가능성을 줄이고, 유지보수 비용을 낮춤 </li>
        <li> 캐릭터는 암살 데이터를 ‘소유’하지 않고 ‘소비’만 하도록 역할을 분리해, 구조를 파악하기 쉬워짐 </li>
      </ul>
      <strong>📚 배운 점</strong>
      <ul>
        <li> 세 번의 리팩토링을 거치며, 부딪히고 고쳐 나가는 과정 자체가 설계 감각을 키우는 데 가장 큰 도움이 된다는 걸 깨달음 </li>
      </ul>
    </td>
  </tr>
</table>

---

## 3. Delegate / GC Crash 문제 (UPROPERTY 관리 문제) <a id="t2"></a> [(트러블 슈팅 목록 이동)](#문제-해결-경험트러블-슈팅-목차-이동)

<table>
  <tr>
    <td style="border:2px solid #a8ddff; border-radius:8px; padding:12px 16px; background:#050a12;">
      <strong>🧩 문제</strong><br>
      <ul>
        <li> 게임이 시작되고 60초쯤 이후에 인벤토리의 델리게이트 크래시가 발생 </li>
      </ul>
      <strong>🔍 원인 분석</strong><br>
      <ul>
        <li> TArray<UCInventoryItem*> 컨테이너에 UPROPERTY를 선언하지 않아 GC가 수거해가는 것이 문제 </li>
      </ul>
      <strong>🛠 해결</strong>
      <ul>
        <li> 아이템 객체 배열을 UPROPERTY를 선언해 GC가 배열 및 그 내부 객체를 올바르게 추적하도록 변경 </li>
      </ul>
      <strong>✅ 결과</strong><br>
      <ul>
        <li> 장시간 플레이 및 인벤토리 갱신이 반복되는 상황에서도 더 이상 델리게이트 Broadcast 시점 크래시가 발생하지 않게 됨 </li>
      </ul>
      <strong>📚 배운 점</strong>
      <ul>
        <li> 기능을 단순히 사용하는 것이 아니라 내부 동작과 원리와 제약을 이해한 뒤 사용하는 것이 중요함을 체감함 </li>
      </ul>
    </td>
  </tr>
</table>

---

## 4. 과도한 FSM 의존으로 인한 상태 관리 문제 <a id="t3"></a> [(트러블 슈팅 목록 이동)](#문제-해결-경험트러블-슈팅-목차-이동)

<table>
  <tr>
    <td style="border:2px solid #4fa3ff; border-radius:8px; padding:12px 16px; background:#050812;">
      <strong>🧩 문제</strong><br>
      <img width="379" height="223" alt="image" src="https://github.com/user-attachments/assets/c69df04f-8936-4bc0-bc67-a7e2e51536ac" />
      <ul>
        <li> 여러 AI가 플레이어를 둘러싸는 전투에서, 상태 전이 조건이 충돌하며 AI가 설계와 다른 움직임을 보임 </li>
      </ul>
      <strong>🔍 원인 분석</strong></br>
      <img width="379" height="223" alt="image" src="https://github.com/user-attachments/assets/998da6e4-a97e-4f5f-85f3-0134c38ce69b" />
      <ul>
        <li> 과도하게 FSM으로 세부 동작을 관리해 방어 코드 의존도가 높아졌고, 해당 방어 코드 누락이 원인이었음 </li>
      </ul>
      <strong>🛠 해결</strong>
      <ul>
        <li> 세부 동작은 FSM 밖에서 처리하도록 분리하고, FSM은 큰 흐름만 관리하도록 구조를 변경함 </li>
        <li> 군집 전투는 중앙 토큰 방식으로 통제해 FSM 영향에서 분리함 </li>
      </ul>
      <strong>✅ 결과</strong><br>
      <ul>
        <li> AI 움직임이 안정적으로 유지되어 전투의 체감 품질이 개선됨 </li>
        <li> 불필요한 방어 코드들이 제거되며 로직 가독성이 향상됨 </li>
        <li> 중앙 제어 구조로 군집 전투 파라미터 조정이 용이해짐 </li>
      </ul>
      <strong>📚 배운 점</strong>
      <ul>
        <li>모든 세부 동작을 FSM 하나로 관리하려는 접근은 유지보수가 어렵고 오류 가능성이 높다는 것을 알게 됨</li>
        <li>FSM은 큰 흐름을 담당하고, 세부 동작은 별도 로직·토큰·블랙보드와 조합하는 방식이 더 적절함을 깨달음</li>
      </ul>
    </td>
  </tr>
</table>

---

</br>

# 📘프로젝트에서 얻은 것 [(목차 이동)](#목차)

| 번호 | 얻은 경험 |
|------|-----------|
| 1 | [구조 설계 & 디자인 패턴 활용 역량](#gain-drawcall) |
| 2 | [데이터 중심 구조 설계 경험](#gain-ue-arch) |
| 3 | [액션 게임 콘텐츠 개발 경험](#gain-cpp-resource) |
| 4 | [MVVM 기반 UI 구조 설계 역량](#gain-gt-rt) |

---

### 1. 구조 설계 & 디자인 패턴 활용 역량 <a id="gain-drawcall"></a> [(⬆표로 이동)](#프로젝트에서-얻은-것-목차-이동)

옵저버, 팩토리, 커맨드 등의 디자인 패턴을 실제 코드에 적용해 보면서 플레이어·아이템·UI 등 객체의 책임을 명확히 나누고, 이후 확장이 쉬운 계층 구조를 설계하는 능력을 키웠습니다.

그 과정에서 한 클래스에 기능이 과도하게 몰리지 않도록 조절하고, 읽기 쉽고 유지보수가 용이한 구조를 의식적으로 만드는 습관을 갖게 되었습니다.

---

### 2. 데이터 중심 구조 설계 경험 <a id="gain-ue-arch"></a> [(⬆표로 이동)](#프로젝트에서-얻은-것-목차-이동)

데이터 에셋과 테이블을 활용해 코드 수정 없이도 밸런스 수치를 조정·운영할 수 있는 데이터 구조를 설계했습니다.

이를 통해 데이터의 정의·등록·관리 책임을 코드와 분리하는 감각을 익혔고 운영·밸런스 관점에서 다루기 쉬운 구조가 무엇인지에 대해 고민할 수 있었습니다.

---

### 3. 액션 게임 콘텐츠 개발 경험 <a id="gain-cpp-resource"></a> [(⬆표로 이동)](#프로젝트에서-얻은-것-목차-이동)

이펙트 회전 방향 계산, 커맨드 입력 구조, 상태 중첩 관리, 피격 이벤트 처리 등 액션 게임 전투 콘텐츠를 직접 구현하며, 그 과정에서 필요한 수학적 개념, 엔진 기능 활용 능력, 실전 노하우를 함께 쌓았습니다.

---

### 4. MVVM 기반 UI 구조 설계 역량 <a id="gain-gt-rt"></a> [(⬆표로 이동)](#프로젝트에서-얻은-것-목차-이동)

이전 유니티 프로젝트에서는 UI와 로직이 강하게 결합되어, UI 코드가 쉽게 비대해지는 문제가 있었습니다.

이번에는 MVVM 패턴을 도입해 체력 변화 같은 게임 로직을 뷰모델을 통한 이벤트 연동 구조로 분리했고 그 과정에서 UI를 보다 유연하고 안전하게 설계하는 방법을 체감했습니다.

</br>

# 📘개발 계기 [(목차 이동)](#목차)
### 1. 언리얼 엔진을 ‘개발자 관점’에서 익히기 위해

사용하기 편한 언리얼의 블루프린트·에디터 기능 뒤에서 어떤 구조와 흐름이 돌아가는지 궁금했습니다.  
그래서 직접 3D 액션 게임을 만들며 C++ API와 액터·컴포넌트·컨트롤러 구조를 몸으로 익히고자 했습니다.

### 2. Unity 3D 액션 RPG를 포팅하며 전투 설계를 고도화하기 위해

이전에 Unity로 만들었던 3D 액션 RPG 전투 시스템을 언리얼로 포팅하면서, 단순 이식이 아니라 타격 판정·상태·카메라·연출을 다시 설계해 재사용 가능한 전투 구조로 발전시키고자 했습니다.

### 3. 디자인 패턴 기반의 유연한 구조 설계 능력을 확장하기 위해

상태 패턴, 전략 패턴 등을 실제 코드에 적용해 무기·스킬·AI를 쉽게 추가·교체할 수 있는 구조를 실전에서 연습하고 싶었습니다.  
변경과 확장이 잦은 액션 게임에 맞는 아키텍처를 직접 설계해 보기 위해 시작한 프로젝트입니다.
