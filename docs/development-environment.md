# Development Environment Setup

## Repository Structure

```
EKNMusic/
├── backend/               # Python serverless backend
│   ├── src/
│   ├── tests/
│   ├── docker/
│   └── requirements.txt
├── client/                # Qt C++ desktop application
│   ├── src/
│   ├── tests/
│   └── CMakeLists.txt
├── docs/                  # Project documentation
├── scripts/               # Utility scripts
└── README.md
```

## Backend Development Environment

### Prerequisites

- Python 3.11+
- pip and virtualenv
- Docker Desktop
- AWS CLI (for deployment)
- Git

### Setup Steps

1. **Create Python Virtual Environment**
```bash
cd backend
python -m venv venv

# Windows
venv\Scripts\activate

# macOS/Linux
source venv/bin/activate
```

2. **Install Dependencies**
```bash
pip install -r requirements.txt
pip install -r requirements-dev.txt
```

3. **Configure Environment Variables**
```bash
cp .env.example .env
# Edit .env with your local settings
```

4. **Start Docker Services**
```bash
cd docker
docker-compose up -d
```

5. **Run Database Migrations**
```bash
python manage.py migrate
```

6. **Start Development Server**
```bash
python manage.py runserver
```

### Development Tools

- **IDE**: PyCharm, VS Code with Python extension
- **Linting**: pylint, flake8, black (code formatter)
- **Testing**: pytest
- **API Testing**: Postman, curl
- **Database Client**: DBeaver, pgAdmin

## Desktop Client Development Environment

### Prerequisites

- C++ compiler (platform-specific)
- Qt 6.x SDK with Qt Creator
- CMake 3.16+
- Git

### Platform-Specific Setup

#### Windows
1. Install Visual Studio 2019/2022 with C++ workload
2. Install Qt 6.x with MSVC compiler
3. Install CMake (or use Qt Creator's built-in)
4. Configure Qt Creator with kit

#### macOS
1. Install Xcode from App Store
2. Install Xcode Command Line Tools
3. Install Qt 6.x for macOS
4. Install CMake via Homebrew: `brew install cmake`

#### Linux (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install build-essential cmake git
sudo apt install qt6-base-dev qt6-tools-dev qt6-tools-dev-tools
```

### Building the Client

```bash
cd client
mkdir build
cd build
cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x/platform
cmake --build .
```

### Development Tools

- **IDE**: Qt Creator (recommended), CLion, VS Code with C++ extension
- **Debugger**: GDB (Linux/macOS), MSVC debugger (Windows)
- **Profiler**: Qt Creator's profiler, Valgrind (Linux)
- **UI Designer**: Qt Designer (integrated in Qt Creator)

## Git Workflow

### Branch Strategy

- `main` - Production-ready code
- `develop` - Integration branch
- `feature/*` - Feature branches
- `bugfix/*` - Bug fix branches
- `release/*` - Release preparation branches

### Commit Guidelines

- Use conventional commits format
- Write clear, descriptive commit messages
- Keep commits atomic and focused

Example:
```
feat(backend): add user authentication endpoint
fix(client): resolve connection timeout issue
docs: update API documentation
```

## Code Quality Tools

### Backend (Python)

```bash
# Linting
pylint src/
flake8 src/

# Formatting
black src/

# Type checking
mypy src/

# Testing
pytest tests/
```

### Client (C++)

```bash
# Linting
clang-tidy src/*.cpp

# Formatting
clang-format -i src/*.cpp

# Static analysis
cppcheck src/
```

## Environment Variables

### Backend `.env` File

```
# Database
DATABASE_HOST=localhost
DATABASE_PORT=5432
DATABASE_NAME=eknmusic_dev
DATABASE_USER=postgres
DATABASE_PASSWORD=dev_password

# API
API_SECRET_KEY=your_secret_key_here
JWT_EXPIRATION=3600

# AWS (for deployment)
AWS_REGION=us-east-1
AWS_ACCESS_KEY_ID=
AWS_SECRET_ACCESS_KEY=

# Environment
ENVIRONMENT=development
DEBUG=true
LOG_LEVEL=INFO
```

### Client Configuration

Client should read backend API URL from configuration file or environment:
- Development: `http://localhost:8000`
- Staging: `https://staging-api.eknmusic.com`
- Production: `https://api.eknmusic.com`

## Docker Services

### Backend Services (docker-compose.yml)

```yaml
services:
  postgres:
    - Port: 5432
    - Volume: Database persistence

  backend:
    - Port: 8000
    - Depends on: postgres

  pgadmin (optional):
    - Port: 5050
    - Database management UI
```

### Useful Docker Commands

```bash
# Start all services
docker-compose up -d

# View logs
docker-compose logs -f backend

# Stop all services
docker-compose down

# Rebuild services
docker-compose up -d --build

# Access database shell
docker-compose exec postgres psql -U postgres -d eknmusic_dev
```

## Troubleshooting

### Backend Issues

- **Database connection failed**: Ensure Docker PostgreSQL is running
- **Port already in use**: Change port in docker-compose.yml
- **Import errors**: Verify virtual environment is activated

### Client Issues

- **Qt not found**: Set CMAKE_PREFIX_PATH to Qt installation
- **Build errors**: Verify compiler and Qt version compatibility
- **Runtime errors**: Ensure Qt libraries are in PATH

## Additional Resources

- Qt Documentation: https://doc.qt.io/
- Python Documentation: https://docs.python.org/
- AWS Lambda Documentation: https://docs.aws.amazon.com/lambda/
- PostgreSQL Documentation: https://www.postgresql.org/docs/
