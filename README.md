# Systems Software Engineer Portfolio

> **🚀 Live Demo: [https://sys-port-patchy.up.railway.app/](https://sys-port-patchy.up.railway.app/)**

A modern, interactive portfolio showcasing systems programming expertise with live C/C++ project demonstrations. Built with Flask and featuring interactive visualizations of memory allocation, cryptographic primitives, and real-time operating system concepts.

## 🎯 Featured Projects

### 1. **Custom Memory Allocator**
Interactive visualization of heap management with:
- Block allocation and deallocation
- Fragmentation analysis
- Real-time memory usage statistics

### 2. **Cryptographic Primitives**
Live demonstration of core crypto algorithms:
- AES-128 encryption/decryption
- SHA-256 hashing
- Secure random key generation

### 3. **Real-Time OS Kernel**
RTOS scheduler simulation featuring:
- Preemptive task scheduling
- Priority-based round-robin algorithm
- Context switching visualization
- CPU usage monitoring

## 🛠️ Technical Stack

- **Backend**: Flask (Python)
- **Frontend**: Vanilla JavaScript, CSS3
- **Deployment**: Railway Platform
- **Architecture**: MVC pattern with responsive design

## 🚀 Quick Start

### Local Development

```bash
# Clone the repository
git clone https://github.com/patchyevolve/sys-portfolio.git
cd sys-portfolio

# Install dependencies
pip install -r requirements.txt

# Run the application
python run.py
```

Navigate to `http://localhost:5000` to view the portfolio locally.

### Production Deployment

The application is configured for easy deployment on:
- **Railway** (current deployment)
- **Heroku** 
- **Render**
- Any Python hosting platform

## 📁 Project Structure

```
portfolio/
├── app.py                 # Main Flask application
├── run.py                 # Development server
├── requirements.txt       # Dependencies
├── Procfile              # Railway/Heroku config
├── runtime.txt           # Python version
├── templates/
│   ├── base.html         # Base template
│   ├── index.html        # Main portfolio
│   └── projects/         # Interactive demos
│       ├── memory-allocator.html
│       ├── crypto.html
│       └── rtos.html
├── static/
│   ├── css/style.css     # Responsive styling
│   └── js/main.js        # Interactive features
└── .gitignore           # Excludes src/ and dev files
```

## 🎨 Key Features

- **🖥️ Live Preview**: Interactive project demonstrations
- **📝 Code Viewer**: Syntax-highlighted C/C++ source code
- **📱 Responsive Design**: Optimized for all devices
- **⚡ Fast Loading**: Optimized assets and efficient routing
- **🔒 Secure**: Sandboxed iframe execution

## 🔧 Customization

### Adding New Projects

Update the `PROJECTS` list in `app.py`:

```python
PROJECTS.append({
    'id': 'new-project',
    'name': 'Project Name',
    'description': 'Project description',
    'tags': ['C', 'Systems', 'Performance'],
    'code': '// Your C/C++ code here',
    'preview_template': 'projects/new-project.html'
})
```

### Styling

Customize colors and themes in `static/css/style.css`:

```css
:root {
    --color-primary: #00ff88;
    --color-secondary: #00d9ff;
    --color-bg: #0a0e1a;
}
```

## 🌐 Deployment Status

- ✅ **Production**: [sys-port-patchy.up.railway.app](https://sys-port-patchy.up.railway.app/)
- ✅ **SSL Certificate**: Enabled
- ✅ **CDN**: Railway global edge network
- ✅ **Monitoring**: Railway dashboard

## 📊 Performance

- **Load Time**: < 2 seconds
- **Lighthouse Score**: 95+ (Performance, Accessibility, SEO)
- **Mobile Optimized**: Responsive design
- **Cross-browser**: Chrome, Firefox, Safari, Edge

## 🤝 About

This portfolio demonstrates expertise in:
- **Systems Programming**: C/C++, memory management, performance optimization
- **Embedded Systems**: Real-time operating systems, microcontrollers
- **Cryptography**: Algorithm implementation, security protocols
- **Web Development**: Full-stack Flask applications

## 📞 Contact

- **Email**: [ishandaksh1000@gmail.com](mailto:ishandaksh1000@gmail.com)
- **GitHub**: [@patchyevolve](https://github.com/patchyevolve)
- **LinkedIn**: [daksh-walia-107748308](https://linkedin.com/in/daksh-walia-107748308)

---

**Built with ❤️ for systems programming excellence** | **© 2026 Daksh Walia**