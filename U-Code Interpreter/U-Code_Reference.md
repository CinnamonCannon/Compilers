# U-Code 명령어 레퍼런스

이 문서는 `ucodei.cpp` 소스 코드를 기반으로 분석한 U-Code 명령어 집합에 대한 레퍼런스입니다.

## 1. 명령어 형식
U-Code 명령어는 다음과 같은 형식을 가집니다.
```
Label Opcode Operand1 Operand2 Operand3
```
*   **Label**: 선택 사항이며, 점프 명령어의 대상이 됩니다.
*   **Opcode**: 명령어 코드 (예: `lod`, `str`, `add` 등)
*   **Operand**: 명령어에 따라 0개에서 3개까지의 피연산자를 가집니다.

## 2. 명령어 목록

### 2.1. 산술 및 논리 연산 (Arithmetic & Logical)
스택의 상위 요소들을 사용하여 연산을 수행하고 결과를 스택에 푸시합니다.

| 명령어 | 피연산자 | 설명 | 동작 (Stack) |
| :--- | :---: | :--- | :--- |
| `notop` | 0 | 논리 부정 (Logical NOT) | `push(!pop())` |
| `neg` | 0 | 부호 반전 (Negation) | `push(-pop())` |
| `add` | 0 | 덧셈 | `v2=pop, v1=pop, push(v1 + v2)` |
| `sub` | 0 | 뺄셈 | `v2=pop, v1=pop, push(v1 - v2)` |
| `mult` | 0 | 곱셈 | `v2=pop, v1=pop, push(v1 * v2)` |
| `div` | 0 | 나눗셈 (몫) | `v2=pop, v1=pop, push(v1 / v2)` |
| `mod` | 0 | 나머지 연산 | `v2=pop, v1=pop, push(v1 % v2)` |
| `and` | 0 | 논리 곱 (AND) | `v2=pop, v1=pop, push(v1 & v2)` |
| `or` | 0 | 논리 합 (OR) | `v2=pop, v1=pop, push(v1 | v2)` |
| `gt` | 0 | 크다 (Greater Than) | `v2=pop, v1=pop, push(v1 > v2)` |
| `lt` | 0 | 작다 (Less Than) | `v2=pop, v1=pop, push(v1 < v2)` |
| `ge` | 0 | 크거나 같다 (Greater Equal) | `v2=pop, v1=pop, push(v1 >= v2)` |
| `le` | 0 | 작거나 같다 (Less Equal) | `v2=pop, v1=pop, push(v1 <= v2)` |
| `eq` | 0 | 같다 (Equal) | `v2=pop, v1=pop, push(v1 == v2)` |
| `ne` | 0 | 같지 않다 (Not Equal) | `v2=pop, v1=pop, push(v1 != v2)` |
| `inc` | 0 | 증가 (Increment) | `v=pop, push(++v)` |
| `dec` | 0 | 감소 (Decrement) | `v=pop, push(--v)` |

### 2.2. 스택 조작 (Stack Manipulation)
| 명령어 | 피연산자 | 설명 | 동작 |
| :--- | :---: | :--- | :--- |
| `dup` | 0 | 스택 상단 요소 복제 | `v=pop, push(v), push(v)` |
| `swp` | 0 | 스택 상단 두 요소 교환 | `v2=pop, v1=pop, push(v2), push(v1)` |
| `ldc` | 1 | 상수 로드 (Load Constant) | `push(Operand1)` |

### 2.3. 메모리 및 변수 접근 (Memory & Variable Access)
변수는 Lexical Level(정적 깊이) 차이와 Offset으로 접근합니다.

| 명령어 | 피연산자 | 설명 | 동작 |
| :--- | :---: | :--- | :--- |
| `lod` | 2 | 변수 값 로드 (Load) | `push(Stack[Addr(Op1, Op2)])` |
| `str` | 2 | 변수 값 저장 (Store) | `Stack[Addr(Op1, Op2)] = pop()` |
| `lda` | 2 | 변수 주소 로드 (Load Address) | `push(Addr(Op1, Op2))` |
| `ldi` | 0 | 간접 로드 (Load Indirect) | `addr=pop, push(Stack[addr])` |
| `sti` | 0 | 간접 저장 (Store Indirect) | `val=pop, addr=pop, Stack[addr]=val` |

*   `Op1`: Lexical Level Difference (현재 레벨 - 변수 정의 레벨)
*   `Op2`: Offset (변수의 위치)

### 2.4. 제어 흐름 (Control Flow)
| 명령어 | 피연산자 | 설명 | 동작 |
| :--- | :---: | :--- | :--- |
| `ujp` | 1 | 무조건 점프 (Unconditional Jump) | `PC = LabelAddress` |
| `tjp` | 1 | 참일 때 점프 (True Jump) | `if (pop()) PC = LabelAddress` |
| `fjp` | 1 | 거짓일 때 점프 (False Jump) | `if (!pop()) PC = LabelAddress` |
| `chkh` | 1 | 상한 체크 (Check High) | `v=pop, if(v > Op1) Error, push(v)` |
| `chkl` | 1 | 하한 체크 (Check Low) | `v=pop, if(v < Op1) Error, push(v)` |
| `nop` | 0 | 아무 동작 안 함 (No Operation) | - |

### 2.5. 프로시저 및 함수 (Procedure & Function)
| 명령어 | 피연산자 | 설명 | 동작 |
| :--- | :---: | :--- | :--- |
| `ldp` | 0 | 프로시저 호출 준비 (Load Parameters) | 스택 프레임 준비 |
| `call` | 1 | 프로시저 호출 (Call) | `PC = LabelAddress` (Return Addr 저장) |
| `ret` | 0 | 프로시저 복귀 (Return) | 호출자에게 복귀, 스택 프레임 해제 |
| `retv` | 0 | 값 반환 복귀 (Return Value) | `val=pop`, 복귀 후 `push(val)` |
| `proc` | 3 | 프로시저 시작 선언 | 스택 프레임 초기화 |
| `bgn` | 1 | 프로그램 시작 | `SP = SP + Op1` (전역 변수 공간 확보) |
| `end` | 0 | 프로그램 종료 | 실행 종료 |

*   `proc` 피연산자:
    *   `Op1`: 파라미터 크기 + 로컬 변수 크기
    *   `Op2`: 블록 번호 (Base)
    *   `Op3`: Static Level (Lexical Level)

### 2.6. 기타 (Others)
| 명령어 | 피연산자 | 설명 | 동작 |
| :--- | :---: | :--- | :--- |
| `sym` | 2 | 심볼 정보 (Symbol) | 실행 시 동작 없음 (디버깅/정보용) |
| `dump` | 0 | 스택 덤프 (Dump) | 현재 스택 상태 출력 |

## 3. 미리 정의된 프로시저 (Predefined Procedures)
`call` 명령어로 호출할 수 있는 내장 함수들입니다.

*   `read`: 정수 입력 (`call read`)
*   `write`: 정수 출력 (`call write`)
*   `lf`: 줄바꿈 출력 (`call lf`)
