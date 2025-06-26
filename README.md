# MicrochipStudio_ATmega128
통신 및 ATmega128 개별 포트 제어
타이머 인터럽트 및 485 통신 구현

## 사용 기술
Microchip Studio (AVR ATmega128)
C Language
RS485 통신

## 기능 소개
UART 통신 테스트(RS485)
인터럽트 기반 타이머 설정
포트 및 핀 개별 제어
기존에 tx data 개별로 보낸것을 한 문장으로 단축시키는 함수 개발

# 🔧 MicrochipStudio_ATmega128

[![Last Commit](https://img.shields.io/github/last-commit/Kujabin/MicrochipStudio_ATmega128)](https://github.com/Kujabin/MicrochipStudio_ATmega128/commits/main)
[![Repo Size](https://img.shields.io/github/repo-size/Kujabin/MicrochipStudio_ATmega128)](https://github.com/Kujabin/MicrochipStudio_ATmega128)
[![Top Language](https://img.shields.io/github/languages/top/Kujabin/MicrochipStudio_ATmega128)](https://github.com/Kujabin/MicrochipStudio_ATmega128)
[![License](https://img.shields.io/github/license/Kujabin/MicrochipStudio_ATmega128)](LICENSE)

---

## 📌 프로젝트 설명

이 프로젝트는 **Microchip Studio** 기반으로 제작된 ATmega128용 테스트 코드입니다.  
UART 통신, 인터럽트 타이머 설정 등을 실습용으로 구성했습니다.

---

## 🧰 사용 기술

- Microchip Studio 7.0
- AVR ATmega128
- C Language
- Git, GitHub

---

## 📂 폴더 구조

MicrochipStudio_ATmega128/
├── Microchip_Test_0605/
│ ├── main.c
│ ├── Uart.c / Uart.h
│ ├── Debug/ (생성됨)
├── Microchip_Test_0605.atsln
└── README.md

yaml
복사
편집

---

## 🚀 실행 방법

1. Microchip Studio에서 `.atsln` 파일 열기
2. 빌드 후 ATmega128 보드에 업로드
3. UART 출력 확인 (115200 bps 등 설정에 맞게)

---

## 📝 라이선스

이 저장소는 [MIT License](./LICENSE)를 따릅니다.

---

## 🙋‍♀️ 만든 사람

- GitHub: [@Kujabin](https://github.com/Kujabin)
