#!/usr/bin/env python3
"""
Simple script to run the Flask portfolio application
"""

import os
import sys
from app import app

if __name__ == '__main__':
    # Set environment variables for development
    os.environ['FLASK_ENV'] = 'development'
    os.environ['FLASK_DEBUG'] = '1'
    
    print("🚀 Starting Interactive Portfolio...")
    print("📍 Open your browser to: http://localhost:5000")
    print("⚡ Press Ctrl+C to stop the server")
    print("-" * 50)
    
    try:
        app.run(host='0.0.0.0', port=5000, debug=True)
    except KeyboardInterrupt:
        print("\n👋 Portfolio server stopped. Thanks for using it!")
        sys.exit(0)