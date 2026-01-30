# Interactive Developer Portfolio - Flask Version

A modern, interactive developer portfolio built with Flask that showcases your projects with live demos, terminal emulation, and code viewing capabilities.

## Features

- **Interactive Terminal**: Run commands and see output in a real terminal interface
- **Live Project Previews**: View your projects running in embedded iframes
- **Code Viewer**: Display source code with syntax highlighting
- **Responsive Design**: Works perfectly on desktop and mobile
- **Modern UI**: Terminal-inspired design with smooth animations
- **Easy Customization**: Simple project configuration and theming

## Quick Start

### 1. Install Dependencies

```bash
pip install -r requirements.txt
```

### 2. Run the Application

```bash
python app.py
```

### 3. Open in Browser

Navigate to `http://localhost:5000` to see your portfolio in action!

## Project Structure

```
portfolio/
├── app.py                 # Main Flask application
├── requirements.txt       # Python dependencies
├── templates/
│   ├── base.html         # Base template
│   ├── index.html        # Main portfolio page
│   └── projects/         # Project preview templates
│       ├── calculator.html
│       ├── todo.html
│       └── weather.html
├── static/
│   ├── css/
│   │   └── style.css     # Main stylesheet
│   └── js/
│       └── main.js       # Interactive JavaScript
└── README.md
```

## Customization

### Adding New Projects

1. **Update the PROJECTS list in `app.py`:**

```python
PROJECTS.append({
    'id': 'my-new-project',
    'name': 'My New Project',
    'description': 'Description of your project',
    'tags': ['React', 'API', 'CSS'],
    'tabs': ['terminal', 'code', 'preview'],
    'code': '''// Your project code here
function myProject() {
    console.log("Hello World!");
}''',
    'preview_template': 'projects/my-project.html'
})
```

2. **Create a preview template in `templates/projects/my-project.html`:**

```html
<!DOCTYPE html>
<html>
<head>
    <style>
        /* Your project styles */
    </style>
</head>
<body>
    <!-- Your project HTML -->
    <script>
        // Your project JavaScript
    </script>
</body>
</html>
```

### Customizing the Design

**Colors**: Edit CSS variables in `static/css/style.css`:

```css
:root {
    --color-primary: #00ff88;      /* Main accent color */
    --color-secondary: #00d9ff;    /* Secondary accent */
    --color-bg: #0a0e1a;           /* Background color */
    /* ... more variables */
}
```

**Fonts**: Update the Google Fonts link in `templates/base.html` and CSS variables.

### Adding Terminal Commands

Add new commands in the `terminal_command()` function in `app.py`:

```python
commands = {
    'mycommand': lambda: 'Output from my custom command',
    # ... existing commands
}
```

### Personal Information

Update the following in `templates/index.html`:

- Hero section: Name, title, description
- Contact section: Email and GitHub links
- Skills section: Your technologies and tools
- Navigation: Customize menu items

## Deployment

### Local Development

```bash
python app.py
```

### Production Deployment

**Using Gunicorn:**

```bash
pip install gunicorn
gunicorn -w 4 -b 0.0.0.0:8000 app:app
```

**Using Docker:**

Create a `Dockerfile`:

```dockerfile
FROM python:3.9-slim

WORKDIR /app
COPY requirements.txt .
RUN pip install -r requirements.txt

COPY . .

EXPOSE 5000
CMD ["python", "app.py"]
```

**Deploy to Heroku, Railway, or any Python hosting service.**

## Features Explained

### Interactive Terminal

- Type commands and see real output
- Project-specific commands
- Command history and auto-scroll
- Realistic terminal appearance

### Code Viewer

- Syntax highlighting
- Copy-to-clipboard functionality
- File name display
- Scrollable code blocks

### Live Previews

- Embedded project demos
- Sandboxed iframe execution
- Browser-like interface
- Full interactivity

### Responsive Design

- Mobile-friendly layout
- Touch-optimized interactions
- Flexible grid system
- Smooth animations

## Browser Support

- Chrome/Edge 88+
- Firefox 85+
- Safari 14+
- Mobile browsers

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

## License

MIT License - feel free to use this for your own portfolio!

## Support

If you encounter any issues or have questions:

1. Check the browser console for errors
2. Ensure all files are properly uploaded
3. Verify Flask is running without errors
4. Test in different browsers

---

**Built with ❤️ for developers who love to showcase their work**

Happy coding! 🚀