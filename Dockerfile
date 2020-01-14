FROM spmallick/opencv-docker:opencv
RUN command curl -sSL https://rvm.io/pkuczynski.asc | gpg --import - && \
      \curl -sSL https://get.rvm.io | bash -s stable
RUN bash -c "source /etc/profile.d/rvm.sh && rvm install ruby-2.5"
