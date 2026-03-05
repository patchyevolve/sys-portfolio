"""
Static site generator for the portfolio.
Uses Flask's test client to pre-render all routes into static HTML files.
"""
import os
import shutil
from app import app, PROJECTS

BUILD_DIR = '_site'


def build():
    # Clean previous build
    if os.path.exists(BUILD_DIR):
        shutil.rmtree(BUILD_DIR)
    os.makedirs(BUILD_DIR)

    client = app.test_client()

    # 1. Render the main index page
    resp = client.get('/')
    write_file(os.path.join(BUILD_DIR, 'index.html'), resp.data)
    print('Built: /index.html')

    # 2. Render project preview pages
    for project in PROJECTS:
        pid = project['id']
        resp = client.get(f'/project/{pid}/preview')
        if resp.status_code == 200:
            dest = os.path.join(BUILD_DIR, 'project', pid, 'preview', 'index.html')
            write_file(dest, resp.data)
            print(f'Built: /project/{pid}/preview/index.html')

    # 3. Copy source code files
    for project in PROJECTS:
        for src_file in project.get('code_files', []):
            src_path = os.path.join('.', src_file)
            if os.path.isfile(src_path):
                dest = os.path.join(BUILD_DIR, 'source', src_file)
                os.makedirs(os.path.dirname(dest), exist_ok=True)
                shutil.copy2(src_path, dest)
                print(f'Copied source: /source/{src_file}')

    # 4. Copy static assets (CSS, JS)
    static_src = os.path.join('.', 'static')
    static_dest = os.path.join(BUILD_DIR, 'static')
    if os.path.isdir(static_src):
        shutil.copytree(static_src, static_dest)
        print('Copied: /static/')

    print(f'\nBuild complete! Output in {BUILD_DIR}/')


def write_file(path, data):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, 'wb') as f:
        f.write(data)


if __name__ == '__main__':
    build()
