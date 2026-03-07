# Systems Software Engineer Portfolio

> **Live Demo:** [https://sysportfolio.netlify.app/](https://sysportfolio.netlify.app/)

Interactive portfolio built with Flask to showcase systems programming projects. The app presents project metadata, serves real source files in a read-only code viewer, and renders interactive previews for each project.

## Highlights

- Flask-based portfolio UI with project cards, code tabs, and iframe previews.
- Source-file-backed code viewer (`/source/<path>`) instead of hardcoded code snippets.
- Project metadata API endpoints:
  - `GET /api/projects`
  - `GET /api/projects/<project_id>`
- Static-site build script (`build.py`) that pre-renders pages to `_site/`.

## Featured Projects

The current project catalog includes:

1. **Custom Memory Allocator** (`memory-allocator`)
2. **Cryptographic Primitives** (`crypto-primitives`)
3. **Real-Time OS Kernel** (`embedded-rtos`)
4. **Stack-based VM** (`simple-vm`)
5. **Lisp Interpreter** (`lisp-interpreter`)
6. **C Web Server** (`web-server`)

## Tech Stack

- **Backend:** Flask (Python 3.10+)
- **Frontend:** Jinja templates + vanilla JavaScript/CSS
- **Deployment:** Netlify (live site)
- **Build option:** Flask test-client static export via `build.py`

## Getting Started

### 1) Install dependencies

```bash
pip install -r requirements.txt
```

### 2) Run locally

```bash
python run.py
```

Then open: `http://localhost:5000`

> Alternative: `python app.py`

## API and Routes

- `/` – main portfolio page
- `/api/projects` – all project metadata
- `/api/projects/<project_id>` – single project metadata
- `/project/<project_id>/preview` – project preview HTML
- `/source/<path:filename>` – raw source-file serving for code viewer

## Static Build

Generate a static export into `_site/`:

```bash
python build.py
```

This will:

- Pre-render the index page
- Pre-render each project preview route
- Copy referenced source files into `_site/source/...`
- Copy static assets (if present)

## Project Structure

```text
.
├── app.py
├── run.py
├── build.py
├── requirements.txt
├── Procfile
├── templates/
│   ├── base.html
│   ├── index.html
│   └── projects/
│       ├── memory-allocator.html
│       ├── crypto.html
│       ├── rtos.html
│       ├── vm.html
│       ├── lisp.html
│       └── server.html
└── src/
    ├── memory-allocator/
    ├── crypto-primitives/
    ├── rtos-kernel/
    ├── simple-vm/
    ├── lisp-interpreter/
    └── web-server/
```

## Deployment

This repository is currently deployed on Netlify at [https://sysportfolio.netlify.app/](https://sysportfolio.netlify.app/). For deployment details and platform notes, see [`DEPLOYMENT.md`](DEPLOYMENT.md).

## Contact

- **Email:** [ishandaksh1000@gmail.com](mailto:ishandaksh1000@gmail.com)
- **GitHub:** [@patchyevolve](https://github.com/patchyevolve)
- **LinkedIn:** [daksh-walia-107748308](https://linkedin.com/in/daksh-walia-107748308)
