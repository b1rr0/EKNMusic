# Database Schema

## Overview

PostgreSQL 15+ database schema for EKNMusic backend.

## Design Principles

- Use UUIDs for primary keys (better for distributed systems)
- Include `created_at` and `updated_at` timestamps
- Soft deletes where appropriate (deleted_at)
- Proper indexing for performance
- Foreign key constraints for data integrity
- Use JSONB for flexible metadata storage

## Schema Diagram

```
┌─────────────────┐         ┌─────────────────┐
│     users       │         │   user_sessions │
├─────────────────┤         ├─────────────────┤
│ id (PK)         │◄───────┤ user_id (FK)    │
│ email (UNIQUE)  │         │ token_hash      │
│ username        │         │ expires_at      │
│ password_hash   │         │ created_at      │
│ created_at      │         └─────────────────┘
│ updated_at      │
│ deleted_at      │
└────────┬────────┘
         │
         │ 1:N
         │
┌────────▼────────┐         ┌─────────────────┐
│   music_tracks  │         │  track_metadata │
├─────────────────┤         ├─────────────────┤
│ id (PK)         │◄───────┤ track_id (FK)   │
│ user_id (FK)    │         │ key             │
│ title           │         │ value           │
│ artist          │         │ created_at      │
│ album           │         └─────────────────┘
│ duration        │
│ file_url        │
│ file_size       │
│ format          │
│ metadata (JSONB)│
│ created_at      │
│ updated_at      │
│ deleted_at      │
└─────────────────┘
```

## Tables

### users

User account information.

```sql
CREATE TABLE users (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    email VARCHAR(255) NOT NULL UNIQUE,
    username VARCHAR(100) NOT NULL,
    password_hash VARCHAR(255) NOT NULL,
    email_verified BOOLEAN DEFAULT FALSE,
    created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP,
    deleted_at TIMESTAMP WITH TIME ZONE
);

CREATE INDEX idx_users_email ON users(email) WHERE deleted_at IS NULL;
CREATE INDEX idx_users_username ON users(username) WHERE deleted_at IS NULL;
CREATE INDEX idx_users_created_at ON users(created_at);
```

**Columns:**
- `id`: Unique identifier (UUID)
- `email`: User email (unique, indexed)
- `username`: Display name
- `password_hash`: Hashed password (bcrypt/argon2)
- `email_verified`: Email verification status
- `created_at`: Account creation timestamp
- `updated_at`: Last update timestamp
- `deleted_at`: Soft delete timestamp

### user_sessions

Active user authentication sessions.

```sql
CREATE TABLE user_sessions (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    user_id UUID NOT NULL REFERENCES users(id) ON DELETE CASCADE,
    token_hash VARCHAR(255) NOT NULL UNIQUE,
    refresh_token_hash VARCHAR(255),
    expires_at TIMESTAMP WITH TIME ZONE NOT NULL,
    user_agent TEXT,
    ip_address INET,
    created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP
);

CREATE INDEX idx_sessions_user_id ON user_sessions(user_id);
CREATE INDEX idx_sessions_token_hash ON user_sessions(token_hash);
CREATE INDEX idx_sessions_expires_at ON user_sessions(expires_at);
```

**Columns:**
- `id`: Session identifier
- `user_id`: Reference to users table
- `token_hash`: Hashed access token
- `refresh_token_hash`: Hashed refresh token
- `expires_at`: Token expiration time
- `user_agent`: Client user agent
- `ip_address`: Client IP address
- `created_at`: Session creation time

### music_tracks

Music track information.

```sql
CREATE TABLE music_tracks (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    user_id UUID NOT NULL REFERENCES users(id) ON DELETE CASCADE,
    title VARCHAR(500) NOT NULL,
    artist VARCHAR(500),
    album VARCHAR(500),
    duration INTEGER, -- in seconds
    file_url TEXT NOT NULL,
    file_size BIGINT, -- in bytes
    format VARCHAR(50), -- mp3, flac, wav, etc.
    bitrate INTEGER, -- in kbps
    metadata JSONB, -- flexible metadata storage
    created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP,
    deleted_at TIMESTAMP WITH TIME ZONE
);

CREATE INDEX idx_tracks_user_id ON music_tracks(user_id) WHERE deleted_at IS NULL;
CREATE INDEX idx_tracks_title ON music_tracks(title) WHERE deleted_at IS NULL;
CREATE INDEX idx_tracks_artist ON music_tracks(artist) WHERE deleted_at IS NULL;
CREATE INDEX idx_tracks_album ON music_tracks(album) WHERE deleted_at IS NULL;
CREATE INDEX idx_tracks_created_at ON music_tracks(created_at);
CREATE INDEX idx_tracks_metadata ON music_tracks USING GIN(metadata);
```

**Columns:**
- `id`: Track identifier
- `user_id`: Track owner
- `title`: Track title
- `artist`: Artist name
- `album`: Album name
- `duration`: Track length in seconds
- `file_url`: S3 or storage URL
- `file_size`: File size in bytes
- `format`: Audio format
- `bitrate`: Audio bitrate
- `metadata`: Additional metadata (JSONB)
- `created_at`: Upload timestamp
- `updated_at`: Last modification
- `deleted_at`: Soft delete timestamp

### playlists

User-created playlists (optional feature).

```sql
CREATE TABLE playlists (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    user_id UUID NOT NULL REFERENCES users(id) ON DELETE CASCADE,
    name VARCHAR(255) NOT NULL,
    description TEXT,
    is_public BOOLEAN DEFAULT FALSE,
    created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP,
    deleted_at TIMESTAMP WITH TIME ZONE
);

CREATE INDEX idx_playlists_user_id ON playlists(user_id) WHERE deleted_at IS NULL;
CREATE INDEX idx_playlists_created_at ON playlists(created_at);
```

### playlist_tracks

Many-to-many relationship between playlists and tracks.

```sql
CREATE TABLE playlist_tracks (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    playlist_id UUID NOT NULL REFERENCES playlists(id) ON DELETE CASCADE,
    track_id UUID NOT NULL REFERENCES music_tracks(id) ON DELETE CASCADE,
    position INTEGER NOT NULL,
    added_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP,
    UNIQUE(playlist_id, track_id)
);

CREATE INDEX idx_playlist_tracks_playlist_id ON playlist_tracks(playlist_id);
CREATE INDEX idx_playlist_tracks_track_id ON playlist_tracks(track_id);
CREATE INDEX idx_playlist_tracks_position ON playlist_tracks(playlist_id, position);
```

## Migrations

Use a migration tool for schema versioning:

### Python Options
- **Alembic** (with SQLAlchemy)
- **Django Migrations** (if using Django)
- **Yoyo Migrations**

### Migration Structure

```
migrations/
├── versions/
│   ├── 001_initial_schema.sql
│   ├── 002_add_playlists.sql
│   ├── 003_add_metadata_indexes.sql
│   └── ...
└── README.md
```

### Migration Best Practices

1. Never modify existing migrations
2. Always create new migration for schema changes
3. Test migrations on development database first
4. Include rollback scripts
5. Keep migrations small and focused
6. Document breaking changes

## Data Types

- **UUID**: All primary keys and foreign keys
- **VARCHAR**: Text with length limits
- **TEXT**: Unlimited text
- **INTEGER/BIGINT**: Numeric values
- **BOOLEAN**: True/false flags
- **TIMESTAMP WITH TIME ZONE**: All timestamps
- **JSONB**: Flexible metadata (indexed with GIN)
- **INET**: IP addresses

## Constraints

### Foreign Keys
- All foreign keys include `ON DELETE CASCADE` or `ON DELETE SET NULL`
- Maintain referential integrity

### Unique Constraints
- Email addresses must be unique
- Username can be unique if required
- Token hashes must be unique

### Check Constraints (optional)

```sql
ALTER TABLE music_tracks
ADD CONSTRAINT check_duration_positive
CHECK (duration > 0);

ALTER TABLE music_tracks
ADD CONSTRAINT check_file_size_positive
CHECK (file_size > 0);
```

## Indexes

### Performance Indexes
- Foreign key columns
- Frequently queried columns (email, username)
- Timestamp columns for sorting
- JSONB columns (GIN indexes)

### Partial Indexes
Use partial indexes with `WHERE deleted_at IS NULL` for soft-deleted tables.

## Security Considerations

1. **Password Storage**: Never store plain text passwords
2. **Token Hashing**: Hash tokens before storage
3. **Soft Deletes**: Use for user data recovery
4. **Row Level Security**: Consider RLS for multi-tenant scenarios
5. **Audit Logging**: Consider audit tables for sensitive operations

## Backup Strategy

1. **Automated Backups**: Daily automated backups
2. **Point-in-Time Recovery**: Enable PITR on RDS
3. **Backup Retention**: 30 days minimum
4. **Test Restores**: Regular restore testing
5. **Cross-Region**: Replicate to another region for production

## Performance Optimization

1. **Connection Pooling**: Use connection pooler (PgBouncer)
2. **Query Optimization**: Regular EXPLAIN ANALYZE
3. **Index Maintenance**: Regular VACUUM and ANALYZE
4. **Monitoring**: Track slow queries
5. **Read Replicas**: For read-heavy workloads

## Sample Queries

### Get user with tracks
```sql
SELECT u.*, COUNT(t.id) as track_count
FROM users u
LEFT JOIN music_tracks t ON t.user_id = u.id AND t.deleted_at IS NULL
WHERE u.id = $1 AND u.deleted_at IS NULL
GROUP BY u.id;
```

### Search tracks
```sql
SELECT * FROM music_tracks
WHERE user_id = $1
  AND deleted_at IS NULL
  AND (
    title ILIKE $2 OR
    artist ILIKE $2 OR
    album ILIKE $2
  )
ORDER BY created_at DESC
LIMIT $3 OFFSET $4;
```

### Get playlist with tracks
```sql
SELECT p.*, t.*, pt.position
FROM playlists p
JOIN playlist_tracks pt ON pt.playlist_id = p.id
JOIN music_tracks t ON t.id = pt.track_id
WHERE p.id = $1 AND p.deleted_at IS NULL AND t.deleted_at IS NULL
ORDER BY pt.position;
```
