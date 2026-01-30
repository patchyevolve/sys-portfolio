"""
Configuration file for the Interactive Portfolio
Customize your portfolio settings here
"""

# Personal Information
PERSONAL_INFO = {
    'name': 'Your Name',
    'title': 'Full-Stack Developer',
    'description': 'Building interactive web experiences with modern technologies. Passionate about clean code, innovative solutions, and continuous learning.',
    'email': 'your.email@example.com',
    'github': 'https://github.com/yourusername',
    'linkedin': 'https://linkedin.com/in/yourusername',  # Optional
    'twitter': 'https://twitter.com/yourusername',      # Optional
}

# Skills Configuration
SKILLS = {
    'Frontend': [
        'React.js / Next.js',
        'JavaScript / TypeScript', 
        'HTML5 / CSS3 / Sass',
        'Tailwind CSS',
        'Vue.js'
    ],
    'Backend': [
        'Node.js / Express',
        'Python / Django / Flask',
        'REST APIs',
        'MongoDB / PostgreSQL',
        'GraphQL'
    ],
    'Tools & Others': [
        'Git / GitHub',
        'Docker',
        'VS Code',
        'Webpack / Vite',
        'AWS / Heroku'
    ]
}

# Theme Configuration
THEME = {
    'primary_color': '#00ff88',
    'secondary_color': '#00d9ff', 
    'accent_color': '#ff00aa',
    'background_color': '#0a0e1a',
    'font_primary': 'Space Grotesk',
    'font_mono': 'IBM Plex Mono'
}

# Flask Configuration
class Config:
    SECRET_KEY = 'your-secret-key-here'  # Change this in production
    DEBUG = True
    
class ProductionConfig(Config):
    DEBUG = False
    
class DevelopmentConfig(Config):
    DEBUG = True