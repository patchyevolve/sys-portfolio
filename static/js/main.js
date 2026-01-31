// Portfolio Interactive Features
class Portfolio {
    constructor() {
        this.currentProject = 0;
        this.currentTab = 'preview';
        this.projects = window.PROJECTS || [];
        
        this.init();
    }
    
    init() {
        // Ensure page starts at top
        window.scrollTo(0, 0);
        
        // Debug: Check if projects loaded
        console.log('Projects loaded:', this.projects);
        console.log('Projects count:', this.projects.length);
        
        if (!this.projects || this.projects.length === 0) {
            console.error('❌ No projects loaded! Check projects data.');
            return;
        }
        
        this.setupEventListeners();
        this.loadProject(0);
    }
    
    setupEventListeners() {
        // Project tabs
        document.querySelectorAll('#project-tabs .project-tab').forEach((tab, index) => {
            tab.addEventListener('click', () => this.switchProject(index));
        });
        
        // View tabs
        document.querySelectorAll('#view-tabs .project-tab').forEach(tab => {
            tab.addEventListener('click', () => {
                const tabType = tab.dataset.tab;
                this.switchTab(tabType);
            });
        });
        
        // Copy code button
        const copyBtn = document.getElementById('copy-btn');
        if (copyBtn) {
            copyBtn.addEventListener('click', () => this.copyCode());
        }
        
        // Smooth scrolling for navigation
        document.querySelectorAll('.nav-link').forEach(link => {
            link.addEventListener('click', (e) => {
                e.preventDefault();
                const target = document.querySelector(link.getAttribute('href'));
                if (target) {
                    target.scrollIntoView({ behavior: 'smooth' });
                }
            });
        });
    }
    
    switchProject(index) {
        this.currentProject = index;
        this.currentTab = 'preview'; // Default to preview
        
        // Update active project tab
        document.querySelectorAll('#project-tabs .project-tab').forEach((tab, i) => {
            tab.classList.toggle('active', i === index);
        });
        
        // Reset to preview tab
        document.querySelectorAll('#view-tabs .project-tab').forEach(tab => {
            tab.classList.toggle('active', tab.dataset.tab === 'preview');
        });
        
        this.loadProject(index);
        this.switchTab('preview');
    }
    
    switchTab(tabType) {
        this.currentTab = tabType;
        
        // Update active view tab
        document.querySelectorAll('#view-tabs .project-tab').forEach(tab => {
            tab.classList.toggle('active', tab.dataset.tab === tabType);
        });
        
        // Update active panel
        document.querySelectorAll('.project-panel').forEach(panel => {
            panel.classList.remove('active');
        });
        
        const activePanel = document.getElementById(`${tabType}-panel`);
        if (activePanel) {
            activePanel.classList.add('active');
        }
        
        // Load content based on tab
        if (tabType === 'code') {
            this.loadCode();
        } else if (tabType === 'preview') {
            this.loadPreview();
        }
    }
    
    loadProject(index) {
        if (!this.projects[index]) {
            console.error('Project not found at index:', index);
            return;
        }
        
        const project = this.projects[index];
        console.log('Loading project:', project);
        
        // Update project description
        document.getElementById('project-name').textContent = project.name;
        document.getElementById('project-desc').textContent = project.description;
        
        // Update tags
        const tagsContainer = document.getElementById('project-tags');
        tagsContainer.innerHTML = project.tags.map(tag => 
            `<span class="tag">${tag}</span>`
        ).join('');
        
        // Load preview immediately
        this.loadPreview();
        // Also load code for when user switches tabs
        this.loadCode();
    }
    
    async loadCode() {
        const project = this.projects[this.currentProject];
        const codeContainer = document.getElementById('code-content');
        const filenameHeader = document.getElementById('code-filename');

        if (!project) {
            codeContainer.textContent = '// Error: No project data available';
            return;
        }

        // Clear previous content
        codeContainer.innerHTML = '';
        if (filenameHeader && !filenameHeader.textContent) {
            filenameHeader.textContent = filename;
        }


        // Guard: no files
        if (!project.code_files || project.code_files.length === 0) {
            codeContainer.textContent = '// No code available for this project';
            return;
        }

        // Load each file (read-only editor behavior)
        for (const file of project.code_files) {
            try {
                const res = await fetch(`/source/${file}`);

                if (!res.ok) {
                    throw new Error(`HTTP ${res.status}`);
                }

                const text = await res.text();
                const filename = file.split('/').pop();

                // Update filename header to first file
                if (!filenameHeader.textContent) {
                    filenameHeader.textContent = filename;
                }

                const block = document.createElement('div');
                block.className = 'code-block';

                block.innerHTML = `
                    <div class="code-file-header">${filename}</div>
                    <pre class="code-editor"><code>${this.escapeHtml(text)}</code></pre>
                `;

                codeContainer.appendChild(block);

            } catch (err) {
                console.error('Failed to load file:', file, err);

                codeContainer.innerHTML += `
                    <pre><code>// Failed to load ${file}</code></pre>
                `;
            }
        }
    }

    escapeHtml(str) {
        return str
            .replaceAll('&', '&amp;')
            .replaceAll('<', '&lt;')
            .replaceAll('>', '&gt;');
    }


    
    loadPreview() {
        const project = this.projects[this.currentProject];
        if (!project) {
            console.error('No project found for preview loading');
            return;
        }
        
        console.log('Loading preview for project:', project.name);
        const iframe = document.getElementById('project-iframe');
        const iframeTitle = document.getElementById('iframe-title');
        
        iframeTitle.textContent = project.name;
        
        // Clear previous content and show loading
        iframe.src = 'about:blank';
        
        // Load preview via Flask route with a small delay to ensure iframe is ready
        setTimeout(() => {
            iframe.src = `/project/${project.id}/preview`;
            console.log('Setting iframe src to:', `/project/${project.id}/preview`);
        }, 100);
        
        // Add load event listener to debug iframe loading
        iframe.onload = function() {
            console.log('Iframe loaded successfully for:', project.name);
        };
        
        iframe.onerror = function() {
            console.error('Iframe failed to load for:', project.name);
        };
    }
    
    copyCode() {
        const codeContent = document.getElementById('code-content').textContent;
        const copyBtn = document.getElementById('copy-btn');
        
        navigator.clipboard.writeText(codeContent).then(() => {
            const originalText = copyBtn.textContent;
            copyBtn.textContent = '✓ Copied!';
            setTimeout(() => {
                copyBtn.textContent = originalText;
            }, 2000);
        }).catch(err => {
            console.error('Failed to copy code:', err);
        });
    }
}

// Initialize portfolio when DOM is loaded
document.addEventListener('DOMContentLoaded', () => {
    // Force scroll to top and remove any hash
    window.history.replaceState(null, null, window.location.pathname);
    window.scrollTo(0, 0);
    
    // Small delay to ensure DOM is fully loaded
    setTimeout(() => {
        window.scrollTo(0, 0);
        new Portfolio();
    }, 100);
});

// Smooth scrolling for hero CTA buttons
document.addEventListener('DOMContentLoaded', () => {
    const ctaButtons = document.querySelectorAll('.hero-cta a[href^="#"]');
    ctaButtons.forEach(button => {
        button.addEventListener('click', (e) => {
            e.preventDefault();
            const target = document.querySelector(button.getAttribute('href'));
            if (target) {
                target.scrollIntoView({ behavior: 'smooth' });
            }
        });
    });
});