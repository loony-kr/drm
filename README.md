# DRM 서버

PDF 파일에 대한 DRM 인증서를 발급하는 서버입니다.

## 프로젝트 구조

```
.
├── core/
│   ├── env/         # 환경 변수 관리
│   ├── hmac/        # HMAC 암호화
│   ├── rsa/         # RSA 암호화
│   ├── base64/      # Base64 인코딩
│   ├── http/        # HTTP 서버
│   ├── database/    # 데이터베이스
│   ├── filesystem/  # 파일 시스템
│   └── decrypt/     # 복호화
├── pdf/            # PDF 파일 저장소
├── server.cpp      # 메인 서버
└── .env           # 환경 설정
```

## 설치 및 실행

1. 의존성 설치
```bash
# C++17 컴파일러
# OpenSSL
# nlohmann/json
# httplib
```

2. 환경 설정
```bash
# .env 파일 생성
MASTER_KEY=your_master_key_here
```

3. PDF 준비

암호화 할 PDF 파일을 /input 폴더에 넣으세요. (없으면 생성)

3. 빌드 및 실행
```bash
# 서버 빌드
g++ -std=c++17 server.cpp -o server

# PDF 파일 암호화
./encrypt

# 서버 실행
./server
```

서버는 8080 포트에서 실행됩니다.

## 주요 기능

- PDF 파일에 대한 DRM 인증서 발급
- RSA 암호화를 통한 안전한 키 교환
- HMAC-SHA256을 이용한 마스터 키 기반 인증
- 파일 시스템 기반의 콘텐츠 관리
- 데이터베이스를 통한 파일 메타데이터 관리

## 기술 스택

- C++17
- nlohmann/json: JSON 처리
- httplib: HTTP 서버 구현
- OpenSSL: 암호화 기능 (RSA, HMAC)

### 핵심 컴포넌트

1. **서버 (server.cpp)**
   - HTTP API 엔드포인트 제공
   - 인증서 발급 처리
   - 사용자 액션 로깅

2. **암호화 모듈**
   - RSA 암호화/복호화
   - HMAC-SHA256 해시 생성
   - Base64 인코딩/디코딩

3. **파일 시스템 관리**
   - PDF 파일 저장 및 관리
   - 임시 파일 처리

4. **데이터베이스**
   - 파일 메타데이터 저장
   - 원본 파일명 관리

## 주의사항

- MASTER_KEY는 반드시 안전하게 관리해야 합니다.
- 실제 운영 환경에서는 mock 서버 대신 실제 인증 서버를 연동해야 합니다.
- 파일 시스템 접근 권한을 적절히 설정해야 합니다. 