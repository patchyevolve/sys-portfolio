from flask import Flask, render_template, jsonify, request, send_file, abort
import os

app = Flask(__name__)

# =========================
# Project configuration
# =========================

PROJECTS = [
    {
        'id': 'memory-allocator',
        'name': 'Custom Memory Allocator',
        'description': 'High-performance memory allocator with explicit control over allocation strategies',
        'tags': ['C', 'Memory Management', 'Performance'],
        'tabs': ['code', 'preview'],

        # 🔽 REAL SOURCE FILES (read-only viewer)
        'code_files': [
            'src/memory-allocator/allocator.c',
            'src/memory-allocator/allocator.h',
            'src/memory-allocator/test_allocator.c',
            'src/memory-allocator/Makefile',
        ],

        'preview_template': 'projects/memory-allocator.html'
    },

    {
        'id': 'crypto-primitives',
        'name': 'Cryptographic Primitives',
        'description': 'Implementation of core cryptographic algorithms from scratch',
        'tags': ['C++', 'Cryptography', 'Security'],
        'tabs': ['code', 'preview'],

        'code_files': [
            'src/crypto-primitives/aes.c',
            'src/crypto-primitives/crypto.h',
            'src/crypto-primitives/sha256.c',
        ],

        'preview_template': 'projects/crypto.html'
    },

    {
        'id': 'embedded-rtos',
        'name': 'Real-Time OS Kernel',
        'description': 'Minimal RTOS kernel for embedded systems with preemptive scheduling',
        'tags': ['C', 'RTOS', 'Embedded'],
        'tabs': ['code', 'preview'],

        'code_files': [
            'src/rtos-kernel/scheduler.c',
            'src/rtos-kernel/rtos.h',
        ],

        'preview_template': 'projects/rtos.html'
    }
]

BASE_DIR = os.path.abspath('.')

# =========================
# Routes
# =========================

@app.route('/')
def index():
    """Main portfolio page"""
    print(f"🏠 Loading main page with {len(PROJECTS)} projects")
    return render_template('index.html', projects=PROJECTS)


@app.route('/api/projects')
def get_projects():
    """Return project metadata (no source code embedded)"""
    return jsonify(PROJECTS)


@app.route('/api/projects/<project_id>')
def get_project(project_id):
    """Return a single project"""
    project = next((p for p in PROJECTS if p['id'] == project_id), None)
    if not project:
        return jsonify({'error': 'Project not found'}), 404
    return jsonify(project)


@app.route('/project/<project_id>/preview')
def project_preview(project_id):
    """Render iframe preview"""
    project = next((p for p in PROJECTS if p['id'] == project_id), None)

    if project and 'preview_template' in project:
        try:
            return render_template(project['preview_template'], project=project)
        except Exception as e:
            return f"Template error: {e}", 500

    return "Preview not available", 404


@app.route('/source/<path:filename>')
def serve_source(filename):
    """
    Serve source files as plain text (read-only code viewer)
    """
    full_path = os.path.join(BASE_DIR, filename)

    if not os.path.isfile(full_path):
        abort(404)

    return send_file(full_path, mimetype='text/plain')


# =========================
# App entry point
# =========================

if __name__ == '__main__':
    port = int(os.environ.get('PORT', 5000))
    debug_mode = os.environ.get('FLASK_DEBUG', 'False').lower() == 'true'

    app.run(host='0.0.0.0', port=port, debug=debug_mode)
