# Deployment Guide

## 🎉 **SUCCESSFULLY DEPLOYED!**

**🚀 Live Portfolio**: [https://sys-port-patchy.up.railway.app/](https://sys-port-patchy.up.railway.app/)

Your Flask portfolio is now live and accessible worldwide!

## 📁 Files Included in Repository:

### Essential Application Files:
- `app.py` - Main Flask application
- `requirements.txt` - Python dependencies
- `Procfile` - Heroku deployment configuration
- `runtime.txt` - Python version specification
- `run.py` - Local development server

### Frontend Assets:
- `templates/` - Jinja2 HTML templates
- `static/` - CSS and JavaScript files
- `interactive-portfolio.html` - Original single-file version

### Configuration:
- `.gitignore` - Excludes non-essential files
- `config.py` - Application configuration

## 🚫 Files Excluded (via .gitignore):
- `src/` directory - Educational C/C++ source code
- Development files (FIXES_APPLIED.md, etc.)
- IDE configurations (.vscode/, .idea/)
- Python cache files (__pycache__/)
- Virtual environments (venv/, env/)

## 🌐 Deployment Options:

### 1. Heroku (Recommended)
```bash
# Install Heroku CLI, then:
heroku create your-portfolio-name
git push heroku master
heroku open
```

### 2. Railway
```bash
# Connect your GitHub repo to Railway
# Railway will auto-detect Flask app
```

### 3. Render
```bash
# Connect GitHub repo to Render
# Use: python app.py as start command
```

### 4. Local Development
```bash
# Install dependencies
pip install -r requirements.txt

# Run development server
python run.py
# OR
python app.py
```

## ⚙️ Environment Variables:
- `PORT` - Server port (auto-set by hosting platforms)
- `FLASK_DEBUG` - Set to 'true' for development mode

## 🔧 Production Features:
✅ Gunicorn WSGI server for production  
✅ Environment-based configuration  
✅ Responsive design  
✅ Interactive project demos  
✅ Clean URL structure  
✅ Optimized static file serving  

## 📊 Project Demos Included:
1. **Memory Allocator** - Interactive heap visualization
2. **Cryptographic Primitives** - AES encryption/decryption demo
3. **RTOS Kernel** - Real-time scheduler simulation

Your portfolio is ready to showcase your systems programming skills! 🎉