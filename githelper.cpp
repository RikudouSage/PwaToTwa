#include "githelper.h"

GitHelper::GitHelper()
{
    git_libgit2_init();
}

GitHelper::~GitHelper()
{
    git_libgit2_shutdown();
}

void GitHelper::clone(QString directory)
{
    clone(QDir(directory));
}

void GitHelper::clone(QDir directory)
{
    directory = QDir(directory);
    if (directory.exists()) {
        throw QString("The directory already exists");
    }

    git_repository *repository = nullptr;
    gitError(git_clone(&repository, URL.toStdString().c_str(), directory.absolutePath().toStdString().c_str(), nullptr));
    git_repository_free(repository);
}

void GitHelper::checkout(QString directory)
{
    checkout(QDir(directory));
}

void GitHelper::checkout(QDir directory)
{
    if(!directory.exists()) {
        throw QString("The directory does not exist");
    }

    git_checkout_options options = GIT_CHECKOUT_OPTIONS_INIT;
    options.checkout_strategy = GIT_CHECKOUT_FORCE;

    git_repository *repository = nullptr;
    git_oid commitId;
    git_commit *commit = nullptr;

    gitError(git_repository_open(&repository, directory.absolutePath().toStdString().c_str()));
    gitError(git_oid_fromstr(&commitId, COMMIT.toStdString().c_str()));
    gitError(git_commit_lookup(&commit, repository, &commitId));

    gitError(git_checkout_tree(repository, reinterpret_cast<const git_object *>(commit), &options));
    git_repository_set_head_detached(repository, &commitId);

    git_repository_free(repository);
    git_commit_free(commit);
}

void GitHelper::reinitGitDirectory(QString directory)
{
    reinitGitDirectory(QDir(directory));
}

void GitHelper::reinitGitDirectory(QDir directory)
{
    if(!directory.exists()) {
        throw QString("The directory does not exist");
    }

    QDir gitDir = directory.filePath(".git");
    if(gitDir.exists()) {
        gitDir.removeRecursively();

        git_repository *repository = nullptr;

        gitError(git_repository_init(&repository, directory.absolutePath().toStdString().c_str(), 0));
        git_repository_free(repository);
    }
}

void GitHelper::initialCommit(QString directory)
{
    initialCommit(QDir(directory));
}

void GitHelper::initialCommit(QDir directory)
{
    if(!directory.exists()) {
        throw QString("The directory does not exist");
    }

    git_index *index = nullptr;
    git_repository *repository = nullptr;

    gitError(git_repository_open(&repository, directory.absolutePath().toStdString().c_str()));
    gitError(git_repository_index(&index, repository));

    char *paths[] = {const_cast<char *>(".")};
    git_strarray data = {paths, 1};

    gitError(git_index_add_all(index, &data, GIT_INDEX_ADD_DEFAULT, nullptr, nullptr));
    gitError(git_index_write(index));

    git_signature *signature = nullptr;
    git_oid treeId;
    git_tree *tree = nullptr;

    gitError(git_signature_now(&signature, "Initial Commit", "root@localhost"));
    gitError(git_index_write_tree(&treeId, index));
    gitError(git_tree_lookup(&tree, repository, &treeId));

    git_oid newCommitId;

    git_commit_create(
                &newCommitId,
                repository,
                "HEAD",
                signature,
                signature,
                "UTF-8",
                "Initial commit",
                tree,
                0,
                nullptr
            );

    git_index_free(index);
    git_repository_free(repository);
    git_signature_free(signature);
    git_tree_free(tree);
}

void GitHelper::gitError(int status)
{
    if(status < 0) {
        const git_error *e = giterr_last();
        throw QString::fromUtf8(e->message);
    }
}
