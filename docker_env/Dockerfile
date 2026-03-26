# Start from a lightweight Ubuntu image
FROM ubuntu:24.04

# Set environment variables to make the image non-interactive
ENV DEBIAN_FRONTEND=noninteractive \
    TZ=Etc/UTC \
    LANG=C.UTF-8

# Update and install only essential tools
#RUN apt-get update && \
#    apt-get install -y --no-install-recommends \
#        ca-certificates \
#        curl \
#        wget \
#        vim \
#        net-tools \
#        iputils-ping \
#        git \
#	strace \
#	build-essential \
#    && apt-get clean && \
#    rm -rf /var/lib/apt/lists/*

# Set the default working directory
WORKDIR /app
# Copy local files to container (optional)
# COPY . .
ENV TINI_VERSION v0.19.0
ADD https://github.com/krallin/tini/releases/download/${TINI_VERSION}/tini /tini
RUN chmod +x /tini
ENTRYPOINT ["/tini", "--"]
# Set a default command (can be overridden)
#CMD ["tail", "-f", "/dev/null"]
CMD ["sleep", "infinity"]
