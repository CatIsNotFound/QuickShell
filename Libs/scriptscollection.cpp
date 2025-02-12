#include "scriptscollection.h"

QDir default_dir(".");

ScriptsCollection::ScriptsCollection() {
    path = "";
    collection = nullptr;
}

ScriptsCollection::~ScriptsCollection() {
    if (collection != nullptr) {
        delete collection;
        collection = nullptr;
    }
}

ScriptsCollection::ScriptsCollection(QString fileName) {
    path = "";
    collection = nullptr;
    if (!fileName.isEmpty()) {
        switch (loadCollection(fileName)) {
        case 1:
            qDebug() << "Error: " << fileName << "is a directory, not a file!";
            break;
        case 2:
            qDebug() << "Error: File" << fileName << "is not exist! It will be created.";
            createCollection("collection", "Script Collection Example");
            saveCollection();
            break;
        case 3:
            qDebug() << "Error: File" << fileName << "is not a valid collection file.";
            break;
        }
    }
}

int ScriptsCollection::loadCollection(QString fileName) {
    QFileInfo isExist(fileName);
    if (isExist.isDir()) {
        return 1;
    }

    collection = new QSettings(fileName, QSettings::IniFormat);
    path = fileName;
    if (!isExist.isFile()) {
        return 2;
    }

    // Is script collection file valid?
    bool is_valid = false;
    if (collection->allKeys().count() < 3) {
        return 3;
    }
    for (auto i : collection->allKeys()) {
        if (comparesEqual(i, "Collection/id")) {
            is_valid = true;
            break;
        }
    }
    if (!is_valid) return 3;

    int k;
    for (int i = 0; i < path.length(); i++) {
        if (path[path.length() - i - 1] == '/') {
            k = path.length() - i - 1;
            break;
        }
    }
    default_dir.setCurrent(path.left(k));
    // qDebug() << "Info: File" << fileName << "is loaded succssfully!";
    return 0;
}

void ScriptsCollection::saveCollection() {
    QStringList groups = getGroups();
    QString value = "";
    for (int i = 0; i < groups.count(); i++) {
        value += groups[i];
        if (i < groups.count() - 1) value += ":";
    }
    collection->setValue("Collection/groups", value);
    collection->sync();
}

QStringList ScriptsCollection::getGroups() {
    QStringList children = collection->childGroups();
    QStringList groups;
    for (auto i : children) {
        if (collection->value(i + "/is_group").toBool()) {
            groups.push_back(i);
        }
    }
    return groups;
}

QStringList ScriptsCollection::getItems(QString group) {
    QStringList children = collection->childGroups();
    QStringList items;
    for (auto i : children) {
        if (comparesEqual(collection->value(i + "/group").toString(), group)) {
            items.push_back(i);
        }
    }
    return items;
}

bool ScriptsCollection::findGroup(QString group) {
    QStringList children = getGroups();
    for (auto i : children) {
        if (comparesEqual(i, group)) {
            return true;
        }
    }
    return false;
}

bool ScriptsCollection::findItem(QString group, QString item) {
    QStringList items = getItems(group);
    for (auto i : items) {
        if (comparesEqual(i, item)) {
            return true;
        }
    }
    return false;
}

bool ScriptsCollection::findGroupOrItem(QString key) {
    QStringList children = collection->childGroups();
    for (auto i : children) {
        if (comparesEqual(i, key)) {
            return true;
        }
    }
    return false;
}

void ScriptsCollection::setValue(QString key, QString value) {
    collection->setValue(key, value);
}

QString ScriptsCollection::getValue(QString key) {
    return collection->value(key).toString();
}

void ScriptsCollection::createCollection(QString id, QString name) {
    collection->clear();
    collection->beginGroup("Collection");
    collection->setValue("id", id);
    collection->setValue("name", name);
    collection->setValue("groups", "");
    collection->endGroup();
    createGroup("example", "Sample Scripts", default_dir.absolutePath() + "/scripts");
    createGroup("a", "Example", default_dir.absolutePath() + "/scripts");
    addItem("HelloWorld", "a", "Hello world", "hello_world.sh", "hello_world.md");
    addItem("CountDown", "example", "Count Down", "count_down.sh", "count_down.md");
    addItem("TestArgs", "example", "Test Arguments", "test_args.sh", "test_args.md");
}

void ScriptsCollection::createGroup(QString name, QString fullName, QString path) {
    collection->beginGroup(name);
    collection->setValue("name", fullName);
    collection->setValue("icon", "Default");
    collection->setValue("is_group", 1);
    collection->setValue("path", path);
    collection->endGroup();
}

void ScriptsCollection::addItem(QString name, QString group, QString fullName, QString path, QString descriptionFile) {
    QString groupName = name;
    collection->beginGroup(groupName);
    collection->setValue("name", fullName);
    collection->setValue("group", group);
    collection->setValue("path", path);
    collection->setValue("need_args", "No");
    collection->setValue("args", "");
    collection->setValue("description_file", getPath(group) + descriptionFile);
    collection->endGroup();
}

bool ScriptsCollection::removeItem(QString name, QString group) {
    if (collection->contains(name)) {
        collection->remove(name);
        return true;
    } else {
        return false;
    }
}

QString ScriptsCollection::getGroup(QString name) {
    return collection->value(name + "/group").toString();
}

void ScriptsCollection::setGroup(QString name, QString new_group) {
    if (findItem(name, "group")) {
        collection->setValue(name + "/group", new_group);
    }
}

void ScriptsCollection::setFullName(QString key, QString fullName) {
    if (findGroupOrItem(key)) {
        collection->setValue(key + "/name", fullName);
    }
}

QString ScriptsCollection::getFullName(QString key) {
    if (findGroupOrItem(key)) {
        return collection->value(key + "/name").toString();
    }
    return "";
}


void ScriptsCollection::setPath(QString key, QString path) {
    if (findGroupOrItem(key)) {
        collection->setValue(key + "/path", path);
    }
}

QString ScriptsCollection::getPath(QString key) {
    if (findGroupOrItem(key)) {
        return collection->value(key + "/path").toString();
    }
    return "";
}

int ScriptsCollection::setIcon(QString group, QString iconPath) {
    if (!findGroup(group)) {
        return 1;
    }

    QIcon isIcon(iconPath);
    if (isIcon.isNull()) {
        return 2;
    }

    collection->setValue(group + "/icon", iconPath);
    return 0;
}

QString ScriptsCollection::getIcon(QString group) {
    return ((findGroup(group)) ? collection->value(group + "/icon").toString() : "");
}

void ScriptsCollection::setNeedArgs(QString item, QString group, bool is_need_args) {
    if (findItem(group, item)) {
        QString value = (is_need_args ? "Yes" : "No");
        collection->setValue(item + "/need_args", value);
    }
}

bool ScriptsCollection::getNeedArgs(QString item, QString group) {
    if (findItem(group, item)) {
        QString value = collection->value(item + "/need_args").toString();
        if (comparesEqual(value, "Yes")) {
            return true;
        } else {
            return false;
        }
    }
    return false;
}

void ScriptsCollection::setArgs(QString item, QString group, QString args) {
    if (findItem(group, item)) {
        collection->setValue(item + "/args", args);
    }
}

QString ScriptsCollection::getArgs(QString item, QString group) {
    if (findItem(group, item)) {
        return collection->value(item + "/args").toString();
    }
    return "";
}

int ScriptsCollection::setDescriptionFile(QString item, QString group, QString fileName) {
    if (!findItem(group, item)) {
        return 1;
    }
    QFileInfo isExist(fileName);
    if (!isExist.isFile()) {
        return 2;
    }
    collection->setValue(item + "/description_file", fileName);
    return 0;
}

QString ScriptsCollection::getDescriptionFile(QString item, QString group) {
    if (findItem(group, item)) {
        return collection->value(item + "/description_file").toString();
    }
    return "";
}
