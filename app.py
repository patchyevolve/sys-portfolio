from flask import Flask, render_template, jsonify, request
import json
import os

app = Flask(__name__)

# Project data - you can move this to a separate JSON file later
# Simplified project data for testing
PROJECTS = [
    {
        'id': 'memory-allocator',
        'name': 'Custom Memory Allocator',
        'description': 'High-performance memory allocator with explicit control over allocation strategies',
        'tags': ['C', 'Memory Management', 'Performance'],
        'tabs': ['code', 'preview'],
        'code': '// Custom Memory Allocator\n#include <stddef.h>\n#include <stdint.h>\n\ntypedef struct block {\n    size_t size;\n    struct block* next;\n    int free;\n} block_t;\n\nstatic block_t* heap_start = NULL;',
        'preview_template': 'projects/memory-allocator.html'
    },
    {
        'id': 'crypto-primitives',
        'name': 'Cryptographic Primitives',
        'description': 'Implementation of core cryptographic algorithms from scratch',
        'tags': ['C++', 'Cryptography', 'Security'],
        'tabs': ['code', 'preview'],
        'code': '// AES-128 Implementation\n#include <stdint.h>\n#include <string.h>\n\n// AES S-box\nstatic const uint8_t sbox[256] = {\n    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5\n    // ... full S-box table\n};',
        'preview_template': 'projects/crypto.html'
    },
    {
        'id': 'embedded-rtos',
        'name': 'Real-Time OS Kernel',
        'description': 'Minimal RTOS kernel for embedded systems with preemptive scheduling',
        'tags': ['C', 'RTOS', 'Embedded'],
        'tabs': ['code', 'preview'],
        'code': '// RTOS Task Control Block\ntypedef struct tcb {\n    uint32_t* stack_ptr;\n    uint32_t stack_size;\n    uint8_t priority;\n    uint8_t state;\n    struct tcb* next;\n    char name[16];\n} tcb_t;',
        'preview_template': 'projects/rtos.html'
    }
]

@app.route('/')
def index():
    """Main portfolio page"""
    print(f"🏠 Loading main page with {len(PROJECTS)} projects")
    for project in PROJECTS:
        print(f"   - {project['id']}: {project['name']}")
    return render_template('index.html', projects=PROJECTS)

@app.route('/api/projects')
def get_projects():
    """API endpoint to get all projects"""
    return jsonify(PROJECTS)

@app.route('/api/projects/<project_id>')
def get_project(project_id):
    """API endpoint to get a specific project"""
    project = next((p for p in PROJECTS if p['id'] == project_id), None)
    if project:
        return jsonify(project)
    return jsonify({'error': 'Project not found'}), 404

@app.route('/project/<project_id>/preview')
def project_preview(project_id):
    """Render project preview in iframe"""
    print(f"🔍 Requested preview for project: {project_id}")
    project = next((p for p in PROJECTS if p['id'] == project_id), None)
    print(f"📋 Found project: {project['name'] if project else 'None'}")
    
    if project and 'preview_template' in project:
        print(f"📄 Loading template: {project['preview_template']}")
        try:
            return render_template(project['preview_template'], project=project)
        except Exception as e:
            print(f"❌ Template error: {e}")
            return f"Template error: {e}", 500
    
    print(f"❌ Preview not available for project: {project_id}")
    return "Preview not available", 404

if __name__ == '__main__':
    # Get port from environment variable for deployment platforms like Heroku
    port = int(os.environ.get('PORT', 5000))
    # Get debug mode from environment
    debug_mode = os.environ.get('FLASK_DEBUG', 'False').lower() == 'true'
    
    app.run(host='0.0.0.0', port=port, debug=debug_mode)