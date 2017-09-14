import json

class manifest_node:
    """A manifest_node is a part of the manifest data structure. We can see
    it as an inventory of a "sub-folder" of the lab data path.
    """
    def __init__(self, name, data_list):
        self.name = name
        self.data_list = data_list

    def get_new_data_name(self):
        pattern = 'New {} #{}'
        number = 1
        while self.data_list.count(pattern.format(self.name, number)) > 0:
            number = number + 1
        return pattern.format(self.name, number)

    def add(self, name):
        if self.data_list.count(name) > 0:
            raise ValueError(
                "Manifest node '{}' has already data '{}'".format(
                    self.name, name
                )
            )
        self.data_list.append(name)

    def rename(self, previous_name, new_name):
        self.data_list.remove(previous_name)
        self.data_list.append(new_name)

    def delete(self, name):
        self.data_list.remove(name)

    def to_json(self):
        return self.data_list

class manifest:
    def __init__(self, path, nodes):
        self.path = path
        self.nodes = nodes

    def get_node(self, name):
        return self.nodes[name]

    def to_json(self):
        nodes = {}
        for name, node in self.nodes.items():
            nodes[name] = node.to_json()
        return json.dumps(nodes)

    def save(self):
        with open(self.path, 'w+') as f:
            f.write(self.to_json())

def load_json(json_file_path):
    """Load a JSON file containing a lab data manifest file."""
    with open(json_file_path, 'r') as f:
        json_text = f.read()
        json_data = json.loads(json_text)

        nodes = {}
        for k, v in json_data.items():
            nodes[k] = manifest_node(k, v)

        return manifest(json_file_path, nodes)

