# Deployment Guide

## Overview

This guide covers deploying the EKNMusic backend to AWS and distributing the desktop client.

## Backend Deployment to AWS

### Architecture Components

- **AWS Lambda**: Serverless compute for API
- **Amazon RDS**: PostgreSQL database
- **API Gateway**: HTTP API routing
- **S3**: File storage (music files, assets)
- **CloudFront**: CDN for file delivery
- **Secrets Manager**: Secure credential storage
- **CloudWatch**: Logging and monitoring
- **IAM**: Access control and permissions

### Prerequisites

- AWS Account with appropriate permissions
- AWS CLI installed and configured
- Serverless Framework or AWS SAM CLI
- Python 3.11+ locally
- Git repository

### Infrastructure Setup

#### 1. VPC and Network Configuration

```bash
# Create VPC for database and Lambda functions
# Configure private subnets for RDS
# Configure public/private subnets for Lambda
# Set up NAT Gateway for Lambda internet access
# Configure Security Groups
```

**Security Groups:**
- Lambda security group (outbound to RDS, internet)
- RDS security group (inbound from Lambda only)
- VPC endpoints for AWS services (optional, cost optimization)

#### 2. RDS PostgreSQL Setup

**Configuration:**
- Engine: PostgreSQL 15.x
- Instance: db.t3.micro (dev), db.t3.medium+ (prod)
- Storage: 20GB minimum, enable auto-scaling
- Multi-AZ: Enabled for production
- Backup retention: 7 days minimum
- Encryption: Enabled at rest
- Connection: Private subnet only

**Creation Steps:**
```bash
# Via AWS Console or CloudFormation
# Store master password in Secrets Manager
# Configure parameter group if needed
# Enable automated backups
# Tag resources appropriately
```

#### 3. S3 Bucket Configuration

**Buckets:**
- `eknmusic-files-{env}`: Music file storage
- `eknmusic-backups-{env}`: Database backups
- `eknmusic-lambda-code-{env}`: Lambda deployment packages

**S3 Configuration:**
```json
{
  "Versioning": "Enabled",
  "Encryption": "AES256 or KMS",
  "LifecyclePolicy": {
    "OldVersions": "Delete after 30 days",
    "Backups": "Move to Glacier after 90 days"
  },
  "CORS": {
    "AllowedOrigins": ["https://desktop-client"],
    "AllowedMethods": ["GET", "PUT", "POST"],
    "AllowedHeaders": ["*"]
  }
}
```

#### 4. Secrets Manager Setup

Store sensitive credentials:
```bash
# Database credentials
aws secretsmanager create-secret \
  --name eknmusic/prod/database \
  --secret-string '{
    "host": "xxx.rds.amazonaws.com",
    "port": 5432,
    "dbname": "eknmusic",
    "username": "admin",
    "password": "xxx"
  }'

# JWT secret
aws secretsmanager create-secret \
  --name eknmusic/prod/jwt-secret \
  --secret-string 'your-secret-key'
```

### Serverless Framework Configuration

#### serverless.yml

```yaml
service: eknmusic-backend

provider:
  name: aws
  runtime: python3.11
  region: us-east-1
  stage: ${opt:stage, 'dev'}
  memorySize: 512
  timeout: 30
  environment:
    STAGE: ${self:provider.stage}
    DB_SECRET_ARN: ${self:custom.dbSecretArn}
  vpc:
    securityGroupIds:
      - ${self:custom.lambdaSecurityGroup}
    subnetIds:
      - ${self:custom.privateSubnet1}
      - ${self:custom.privateSubnet2}
  iam:
    role:
      statements:
        - Effect: Allow
          Action:
            - secretsmanager:GetSecretValue
          Resource: ${self:custom.dbSecretArn}
        - Effect: Allow
          Action:
            - s3:GetObject
            - s3:PutObject
          Resource: arn:aws:s3:::eknmusic-files-${self:provider.stage}/*

functions:
  api:
    handler: src/handler.main
    events:
      - httpApi:
          path: /{proxy+}
          method: ANY

plugins:
  - serverless-python-requirements
  - serverless-offline

custom:
  pythonRequirements:
    dockerizePip: true
    layer: true
```

### Deployment Steps

#### 1. Prepare Environment

```bash
# Install dependencies
cd backend
pip install -r requirements.txt

# Run tests
pytest tests/

# Configure environment
export AWS_PROFILE=eknmusic-prod
export STAGE=prod
```

#### 2. Database Migration

```bash
# Connect to RDS (via bastion host or VPN)
# Run migrations
alembic upgrade head

# Or via Lambda function
serverless invoke -f migrate --stage prod
```

#### 3. Deploy Lambda Functions

```bash
# Deploy to development
serverless deploy --stage dev

# Deploy to production
serverless deploy --stage prod

# Deploy single function
serverless deploy function -f api --stage prod
```

#### 4. Configure API Gateway

- Custom domain name (api.eknmusic.com)
- SSL certificate (ACM)
- Rate limiting and throttling
- API key management (if needed)
- CORS configuration
- Request validation

#### 5. CloudFront Setup (Optional)

For file delivery optimization:
```
CloudFront Distribution
├── Origin: S3 bucket (music files)
├── Cache behavior: Based on file type
├── SSL Certificate: Custom domain
└── Geo-restrictions: If needed
```

### Environment Configuration

#### Development
- Single Lambda function
- db.t3.micro RDS instance
- Minimal logging
- No CloudFront

#### Staging
- Mirrors production architecture
- Separate database
- Full logging enabled
- Test CloudFront setup

#### Production
- Multi-AZ RDS
- Production-sized instances
- CloudFront enabled
- Enhanced monitoring
- Alerts configured

### Monitoring and Alerts

#### CloudWatch Metrics
- Lambda invocations, errors, duration
- API Gateway requests, latency, errors
- RDS CPU, connections, storage
- Custom application metrics

#### CloudWatch Alarms
```bash
# Lambda errors
aws cloudwatch put-metric-alarm \
  --alarm-name eknmusic-lambda-errors \
  --alarm-description "Lambda error rate high" \
  --metric-name Errors \
  --threshold 10 \
  --comparison-operator GreaterThanThreshold

# RDS CPU
aws cloudwatch put-metric-alarm \
  --alarm-name eknmusic-rds-cpu \
  --alarm-description "RDS CPU high" \
  --metric-name CPUUtilization \
  --threshold 80 \
  --comparison-operator GreaterThanThreshold
```

#### Logging Strategy
- Structured JSON logging
- Log levels: DEBUG (dev), INFO (staging), WARN (prod)
- Log retention: 7 days (dev), 30 days (prod)
- Centralized log aggregation

### CI/CD Pipeline

#### GitHub Actions Example

```yaml
name: Deploy Backend

on:
  push:
    branches: [main, develop]

jobs:
  deploy:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v2

      - name: Setup Python
        uses: actions/setup-python@v2
        with:
          python-version: '3.11'

      - name: Install dependencies
        run: |
          cd backend
          pip install -r requirements.txt
          pip install -r requirements-dev.txt

      - name: Run tests
        run: pytest backend/tests/

      - name: Configure AWS credentials
        uses: aws-actions/configure-aws-credentials@v1
        with:
          aws-access-key-id: ${{ secrets.AWS_ACCESS_KEY_ID }}
          aws-secret-access-key: ${{ secrets.AWS_SECRET_ACCESS_KEY }}
          aws-region: us-east-1

      - name: Deploy to AWS
        run: |
          cd backend
          serverless deploy --stage prod
```

## Desktop Client Distribution

### Windows Distribution

#### Build Process
```bash
# Build release version
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release

# Bundle dependencies
windeployqt.exe --release --no-translations bin/EKNMusic.exe
```

#### Installer Creation
- **Tool**: NSIS or Inno Setup
- **Include**: Application files, Qt libraries, VC++ redistributables
- **Features**: Install directory selection, start menu shortcuts, uninstaller

#### Code Signing
```bash
# Sign executable
signtool sign /f certificate.pfx /p password /t http://timestamp.digicert.com EKNMusic.exe
```

#### Distribution
- Direct download from website
- Auto-update mechanism (WinSparkle)
- Optional: Microsoft Store

### macOS Distribution

#### Build Process
```bash
# Build release version
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release

# Create app bundle
macdeployqt EKNMusic.app -dmg
```

#### Code Signing and Notarization
```bash
# Sign app
codesign --deep --force --verify --verbose --sign "Developer ID" EKNMusic.app

# Create DMG
hdiutil create -volname "EKNMusic" -srcfolder EKNMusic.app -ov -format UDZO EKNMusic.dmg

# Sign DMG
codesign --sign "Developer ID" EKNMusic.dmg

# Notarize
xcrun notarytool submit EKNMusic.dmg --apple-id "email" --password "app-specific-password"

# Staple ticket
xcrun stapler staple EKNMusic.dmg
```

#### Distribution
- Direct DMG download
- Auto-update mechanism (Sparkle)
- Optional: Mac App Store

### Linux Distribution

#### AppImage
```bash
# Use linuxdeployqt
linuxdeployqt EKNMusic -appimage

# Creates portable AppImage
EKNMusic-x86_64.AppImage
```

#### Debian Package (.deb)
```bash
# Create package structure
mkdir -p debian-package/DEBIAN
mkdir -p debian-package/usr/bin
mkdir -p debian-package/usr/share/applications
mkdir -p debian-package/usr/share/icons

# Create control file
# Build .deb
dpkg-deb --build debian-package eknmusic.deb
```

#### RPM Package
```bash
# Create RPM spec file
# Build RPM
rpmbuild -ba eknmusic.spec
```

#### Distribution
- AppImage for universal compatibility
- .deb for Ubuntu/Debian
- .rpm for Fedora/RHEL
- Distribution via website
- Optional: Snap Store, Flathub

### Auto-Update System

#### Update Server
- Host update manifests on S3/CloudFront
- Manifest format (JSON):
```json
{
  "version": "1.0.0",
  "release_date": "2024-01-01",
  "platforms": {
    "windows": {
      "url": "https://downloads.eknmusic.com/EKNMusic-1.0.0-win64.exe",
      "signature": "...",
      "size": 52428800
    },
    "macos": {
      "url": "https://downloads.eknmusic.com/EKNMusic-1.0.0.dmg",
      "signature": "...",
      "size": 45088768
    },
    "linux": {
      "url": "https://downloads.eknmusic.com/EKNMusic-1.0.0-x86_64.AppImage",
      "signature": "...",
      "size": 48234496
    }
  },
  "release_notes": "Bug fixes and improvements"
}
```

#### Client Update Logic
1. Check for updates on startup
2. Download update in background
3. Verify signature
4. Prompt user to install
5. Install and restart

## Rollback Strategy

### Backend Rollback
```bash
# List deployments
serverless deploy list --stage prod

# Rollback to previous
serverless rollback --timestamp <timestamp> --stage prod
```

### Database Rollback
```bash
# Restore from backup
aws rds restore-db-instance-from-db-snapshot \
  --db-instance-identifier eknmusic-restored \
  --db-snapshot-identifier snapshot-id

# Or rollback migration
alembic downgrade -1
```

### Client Rollback
- Keep previous versions available for download
- Update manifest to point to stable version
- Communicate issue to users

## Post-Deployment Checklist

- [ ] Verify all Lambda functions running
- [ ] Test API endpoints
- [ ] Verify database connectivity
- [ ] Check S3 file uploads/downloads
- [ ] Verify CloudWatch logs appearing
- [ ] Test desktop client connection to API
- [ ] Verify auto-update mechanism
- [ ] Monitor error rates
- [ ] Review costs and optimize if needed
- [ ] Document any deployment issues
