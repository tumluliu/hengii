#!/bin/bash
git add -A .
read message
git commit -m "$message"
git update-server-info
